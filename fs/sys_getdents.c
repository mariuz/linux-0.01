
#include <dirent.h>
#include <errno.h>

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <asm/segment.h>
#include <sys/stat.h>

struct buffer_head* read_file_block(struct m_inode * inode,int block_num);

static int minix_getdents(struct m_inode * inode, struct file * filp,
	struct dirent * dirent, int count)
{
	unsigned int offset,i;
	char c;
	struct buffer_head * bh;
	struct dir_entry * de;

	if (!inode ||/* !inode->i_sb ||*/ !S_ISDIR(inode->i_mode))
		return -EBADF;/*
	info = &inode->i_sb->u.minix_sb;
	if (filp->f_pos & (info->s_dirsize - 1))
		return -EBADF;*/
	while (filp->f_pos < inode->i_size) {
		offset = filp->f_pos & 1023;
		bh = read_file_block(inode,(filp->f_pos)/BLOCK_SIZE);
		if (!bh) {
			filp->f_pos += 1024-offset;
			continue;
		}
		while (offset < 1024 && filp->f_pos < inode->i_size) {
			de = (struct dir_entry *) (offset + bh->b_data);
			offset += sizeof(struct dir_entry);//info->s_dirsize; 
			filp->f_pos += sizeof(struct dir_entry);
			if (de->inode) {
				for (i = 0; i < NAME_LEN; i++)
					if ((c = de->name[i]) != 0)
						put_fs_byte(c,i+dirent->d_name);
					else
						break;
				if (i) {
					put_fs_long(de->inode,&dirent->d_ino);
					put_fs_byte(0,i+dirent->d_name);
					put_fs_word(i,&dirent->d_reclen);
					brelse(bh);
					return i;
				}
			}
		}
		brelse(bh);
	}
	return 0;
}

static int minix_getdents64(struct m_inode * inode, struct file * filp,
	struct dirent64 * dirent, int count)
{
	unsigned int offset,i;
	char c;
	struct buffer_head * bh;
	struct dir_entry * de;

	if (!inode ||/* !inode->i_sb ||*/ !S_ISDIR(inode->i_mode))
		return -EBADF;/*
	info = &inode->i_sb->u.minix_sb;
	if (filp->f_pos & (info->s_dirsize - 1))
		return -EBADF;*/
	while (filp->f_pos < inode->i_size) {
		offset = filp->f_pos & 1023;
		bh = read_file_block(inode,(filp->f_pos)/BLOCK_SIZE);
		if (!bh) {
			filp->f_pos += 1024-offset;
			continue;
		}
		while (offset < 1024 && filp->f_pos < inode->i_size) {
			de = (struct dir_entry *) (offset + bh->b_data);
			offset += sizeof(struct dir_entry);//info->s_dirsize; 
			filp->f_pos += sizeof(struct dir_entry);
			if (de->inode) {
				for (i = 0; i < NAME_LEN; i++)
					if ((c = de->name[i]) != 0)
						put_fs_byte(c,i+dirent->d_name);
					else
						break;
				if (i) {
					put_fs_long(de->inode,&dirent->d_ino);
					put_fs_byte(0,i+dirent->d_name);
					put_fs_word(i,&dirent->d_reclen);
					brelse(bh);
					return i;
				}
			}
		}
		brelse(bh);
	}
	return 0;
}

int sys_getdents(unsigned int fd, struct dirent *dirp, unsigned int count)
{
	struct file * file;
	struct m_inode * inode;
	if (fd >= NR_OPEN || !(file = current->filp[fd]) ||
	    !(inode = file->f_inode))
		return -EBADF;

	verify_area(dirp, sizeof (*dirp));
	return minix_getdents(inode,file,dirp,count);
}

int sys_getdents64(unsigned int fd, struct dirent64 *dirp, unsigned int count)
{
	struct file * file;
	struct m_inode * inode;
	if (fd >= NR_OPEN || !(file = current->filp[fd]) ||
	    !(inode = file->f_inode))
		return -EBADF;

	verify_area(dirp, sizeof (*dirp));
	return minix_getdents64(inode,file,dirp,count);
}

