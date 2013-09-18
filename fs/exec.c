
/*
 *  linux/fs/exec.c
 *
 *  (C) 1991  Linus Torvalds
 *  (C) 2007  Abdel Benamrouche : use elf binary instead of a.out
 * 
 */


#include <errno.h>
#include <sys/stat.h>
#include <elf.h>

#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <asm/segment.h>

extern int sys_exit(int exit_code);
extern int sys_close(int fd);

/*
 * MAX_ARG_PAGES defines the number of pages allocated for arguments
 * and envelope for the new program. 32 should suffice, this gives
 * a maximum env+arg of 128kB !
 */
#define MAX_ARG_PAGES 32

inline void cp_block(const void * from,void * to, int size)
{
int d0,d1,d2;
__asm__ __volatile("pushl $0x10\n\t"
	"pushl $0x17\n\t"
	"pop %%es\n\t"
	"cld\n\t"
	"rep\n\t"
	"movsl\n\t"
	"pop %%es"
	:"=&c" (d0), "=&S" (d1), "=&D" (d2)
	:"0" (size/4),"1" (from),"2" (to)
	:"memory");
}

typedef struct
{
	unsigned long b_entry;
	unsigned long b_size;				//file size
}bin_section;


#include <string.h>

/*
 * create_tables() parses the env- and arg-strings in new user
 * memory and creates the pointer tables from them, and puts their
 * addresses on the "stack", returning the new stack pointer value.
 */
static unsigned long * create_tables(char * p,int argc,int envc)
{
	unsigned long *argv,*envp;
	unsigned long * sp;

	sp = (unsigned long *) (0xfffffffc & (unsigned long) p);
	sp -= envc+1;
	envp = sp;
	sp -= argc+1;
	argv = sp;
	//put_fs_long((unsigned long)envp,--sp);
	//put_fs_long((unsigned long)argv,--sp);
	put_fs_long((unsigned long)argc,--sp);
	while (argc-->0) {
		put_fs_long((unsigned long) p,argv++);
		while (get_fs_byte(p++)) /* nothing */ ;
	}
	put_fs_long(0,argv);
	while (envc-->0) {
		put_fs_long((unsigned long) p,envp++);
		while (get_fs_byte(p++)) /* nothing */ ;
	}
	put_fs_long(0,envp);
	return sp;
}

/*
 * count() counts the number of arguments/envelopes
 */
static int count(char ** argv)
{
	int i=0;
	char ** tmp;

	if ((tmp = argv))
		while (get_fs_long((unsigned long *) (tmp++)))
			i++;

	return i;
}

/*
 * 'copy_string()' copies argument/envelope strings from user
 * memory to free pages in kernel mem. These are in a format ready
 * to be put directly into the top of new user memory.
 */
static unsigned long copy_strings(int argc,char ** argv,unsigned long *page,
		unsigned long p)
{
	int len,i;
	char *tmp;

	while (argc-- > 0) {
		if (!(tmp = (char *)get_fs_long(((unsigned long *) argv)+argc)))
			panic("argc is wrong");
		len=0;		/* remember zero-padding */
		do {
			len++;
		} while (get_fs_byte(tmp++));
		if (p-len < 0)		/* this shouldn't happen - 128kB */
			return 0;
		i = ((unsigned) (p-len)) >> 12;
		while (i<MAX_ARG_PAGES && !page[i]) {
			if (!(page[i]=get_free_page()))
				return 0;
			i++;
		}
		do {
			--p;
			if (!page[p/PAGE_SIZE])
				panic("nonexistent page in exec.c");
			((char *) page[p/PAGE_SIZE])[p%PAGE_SIZE] =
				get_fs_byte(--tmp);
		} while (--len);
	}
	return p;
}

static unsigned long change_ldt(unsigned long text_size,unsigned long * page)
{
	unsigned long code_limit,data_limit,code_base,data_base;
	int i;

	code_limit = text_size+PAGE_SIZE -1;
	code_limit &= 0xFFFFF000;
	data_limit = 0x4000000;
	code_base = get_base(current->ldt[1]);
	data_base = code_base;
	set_base(current->ldt[1],code_base);
	set_limit(current->ldt[1],code_limit);
	set_base(current->ldt[2],data_base);
	set_limit(current->ldt[2],data_limit);
/* make sure fs points to the NEW data segment */
	__asm__("pushl $0x17\n\tpop %%fs"::);
	data_base += data_limit;
	for (i=MAX_ARG_PAGES-1 ; i>=0 ; i--) {
		data_base -= PAGE_SIZE;
		if (page[i])
			put_page(page[i],data_base);
	}
	return data_limit;
}

/*
 * return 1 if ex if a valid elf executable
 * */
int is_valid_elf(Elf32_Ehdr* ex)
{
	if (ex->e_ident[EI_MAG0]!=ELFMAG0 || ex->e_ident[EI_MAG1]!=ELFMAG1 ||
		ex->e_ident[EI_MAG2]!=ELFMAG2 || ex->e_ident[EI_MAG3]!=ELFMAG3){
		printk("not elf format\n");
		return 0;	/*not elf format */
	}

	if (ex->e_type != ET_EXEC || ex->e_machine != EM_386){
		printk("bad elf binary\n");
		return 0;
	}

	return 1;
}

/*
 * bread(dev,ind) is an array of block index
 * So we have to get from this array our wanted index ( variable block)
 * */
struct buffer_head* read_file_block_ind(int dev,int ind,int block_num)
{
	struct buffer_head * bh=NULL,*ih;
	unsigned short block;

	if (!(ih=bread(dev,ind)))
	{
		printk("bad block tab\n");
		return NULL;
	}

	if ((block = *((unsigned short *) (ih->b_data) + block_num))){
		if (!(bh=bread(dev,block))) {
			brelse(ih);
			printk("bad block file\n");
			return NULL;
		}
	}


	brelse(ih);
	return bh;
}

/*
 * if block_num=5, read file at position 5*1024.
 * this work only for minix 1 fs 
 */
struct buffer_head* read_file_block(struct m_inode * inode,int block_num)
{
	struct buffer_head * dind;
	int i;
	unsigned short table;

	 /* 7 1st block can be read directly */
	if (block_num<=6)
	{
		return bread(inode->i_dev,inode->i_zone[block_num]);
	}

	block_num-=7;

	if (block_num<512)
	{
		/* read block at i_zone[7] is an array of block index */
		return read_file_block_ind(inode->i_dev,inode->i_zone[7],block_num);
	}

	block_num-=512;

	if (block_num>512*512)
	{
		panic("Impossibly long executable");
		return NULL;	//just to avoid warning compilation
	}

	/* i_zone[8] => array of array block index */
	if (!(i=inode->i_zone[8]))
		return NULL;
	if (!(dind = bread(inode->i_dev,i)))
		return NULL;
	
	table = *((unsigned short *) dind->b_data+(block_num/512));
	brelse(dind);
	if (block_num>=512)
		block_num-=(block_num-1)*512;
	
	return read_file_block_ind(inode->i_dev,table,block_num);
}


/*  
 *  read an area into %fs:mem.
 */
int copy_section(struct m_inode * inode,Elf32_Off from, Elf32_Addr dest,Elf32_Word size)
{
	struct buffer_head * bh;
	int block_num=from/BLOCK_SIZE;
	int block_offset;			//only for 1st block
	int cp_size;				

	//read fist block
	block_offset=from%BLOCK_SIZE;
	bh=read_file_block(inode,block_num);
	if (!bh) return -1;
	cp_size=(size<BLOCK_SIZE-block_offset)?size:BLOCK_SIZE-block_offset;
	//note we add 3 to size to get it aligned to word boundary
	cp_block(bh->b_data+block_offset,(void*)dest,cp_size+3);
	brelse(bh);
	dest+=cp_size;
	size-=cp_size;
	block_num++;

	//read others blocks
	while(size)
	{
		bh=read_file_block(inode,block_num);
		if (!bh) return -1;
		cp_size=(size<BLOCK_SIZE)?size:BLOCK_SIZE;
		cp_block(bh->b_data,(void*)dest,cp_size+3);
		block_num++;
		size-=cp_size;
		dest+=cp_size;
		brelse(bh);
	};

	return 0;
}

inline int create_bss_section(Elf32_Addr dest,Elf32_Word size)
{
	while (size--)
		put_fs_byte(0,(char *)dest++);
	return 0;
}

/*
 * in linux 0.01 only a.out binary format was supported
 * Today it's hard to compile some programs (like bach) in a.out format
 * So this version of linux 0.01 support elf binary.
 * nb : there is no support of shared library !
 */
int load_elf_binary(struct m_inode *inode, struct buffer_head* bh,
					 bin_section* bs)
{
	Elf32_Ehdr ex;
	Elf32_Shdr sect;
	long nsect;
	int lb=0;
	struct buffer_head* bht=NULL;

	ex = *((Elf32_Ehdr *) bh->b_data);	/* read exec-header */

	/* check header */
	if (!is_valid_elf(&ex)) {
		return -1;
	}

	bs->b_entry=ex.e_entry;
	nsect=ex.e_shnum;
	bs->b_size=ex.e_ehsize+nsect*ex.e_shentsize;
	
	if (nsect<=1){
		printk("bad nb of sections %d\n",nsect);
		return -1;
	}
	
	while(nsect--){
	
		/* load block where is our table section*/
		if ((ex.e_shoff + nsect * ex.e_shentsize)/BLOCK_SIZE != lb)
		{
			printk("");	// gcc  bug : removing this line = gcc not happy !!!
			lb = (ex.e_shoff + nsect * ex.e_shentsize)/BLOCK_SIZE;
			if (bht) brelse(bht);
			bht=read_file_block(inode,lb);
			if (!bht) return -1;
		}

		/* copy this section fo %fs:mem */
		sect=*((Elf32_Shdr *)(bht->b_data + (ex.e_shoff + 
				nsect * ex.e_shentsize)%BLOCK_SIZE ));
	
		if (!sect.sh_size || !sect.sh_addr) continue;
		
		switch(sect.sh_type)
		{
			case SHT_PROGBITS:
				if (copy_section(inode,sect.sh_offset,sect.sh_addr,
					sect.sh_size))
				{
					if (bht) brelse(bht);
					return -1;
				}
				break;
			
			case SHT_NOBITS:	  
				create_bss_section(sect.sh_addr,sect.sh_size);
				break;

			default:
			continue;	  
		}

		/* find binary size */
		if (bs->b_size<sect.sh_addr + sect.sh_size)
			bs->b_size=sect.sh_addr + sect.sh_size;	
	};

	if (bht) brelse(bht);
	return 0;
}

/*
 * 'do_execve()' executes a new program.
 */
int do_execve(unsigned long * eip,long tmp,char * filename,
	char ** argv, char ** envp)
{
	struct m_inode * inode;
	struct buffer_head * bh;
	unsigned long page[MAX_ARG_PAGES];
	int i,argc,envc;
	unsigned long p;
	bin_section bs;

	if ((0xffff & eip[1]) != 0x000f)
		panic("execve called from supervisor mode");
	for (i=0 ; i<MAX_ARG_PAGES ; i++)	/* clear page-table */
		page[i]=0;
	if (!(inode=namei(filename)))		/* get executables inode */
		return -ENOENT;
	if (!S_ISREG(inode->i_mode)) {		/* must be regular file */
		iput(inode);
		return -EACCES;
	}
	i = inode->i_mode;
	if (current->uid && current->euid) {
		if (current->euid == inode->i_uid)
			i >>= 6;
		else if (current->egid == inode->i_gid)
			i >>= 3;
	} else if (i & 0111)
		i=1;
	if (!(i & 1)) {
		iput(inode);
		return -ENOEXEC;
	}
	if (!(bh = bread(inode->i_dev,inode->i_zone[0]))) {
		iput(inode);
		return -EACCES;
	}

	argc = count(argv);
	envc = count(envp);
	p = copy_strings(envc,envp,page,PAGE_SIZE*MAX_ARG_PAGES);
	p = copy_strings(argc,argv,page,p);
	if (!p) {
		for (i=0 ; i<MAX_ARG_PAGES ; i++)
			free_page(page[i]);
		iput(inode);
		return -1;
	}
/* OK, This is the point of no return */
	for (i=0 ; i<32 ; i++)
		current->sig_fn[i] = NULL;
	for (i=0 ; i<NR_OPEN ; i++)
		if ((current->close_on_exec>>i)&1)
			sys_close(i);
	current->close_on_exec = 0;

	free_page_tables(get_base(current->ldt[1]),get_limit(0x0f));
	free_page_tables(get_base(current->ldt[2]),get_limit(0x17));
	
	if (load_elf_binary(inode,bh,&bs)){
		brelse(bh);
		iput(inode);
		return -EACCES;
	}
	brelse(bh);

	if (last_task_used_math == current)
		last_task_used_math = NULL;
	current->used_math = 0;
	p += change_ldt(bs.b_size,page)-MAX_ARG_PAGES*PAGE_SIZE;
	p = (unsigned long) create_tables((char *)p,argc,envc);
	current->brk = bs.b_size;
	current->end_data = bs.b_size;
	current->end_code = bs.b_size;
	current->start_stack = p & 0xfffff000;
	iput(inode);

	i = bs.b_size;
	while (i&0xfff)
		put_fs_byte(0,(char *) (i++));
	
	eip[0] = bs.b_entry;		/* eip, magic happens :-) */
	eip[3] = p;					/* stack pointer */

	return 0;
}
