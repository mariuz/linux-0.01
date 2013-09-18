#ifndef _SYS_STAT_H
#define _SYS_STAT_H

#include <sys/types.h>
#include <stdint.h>

struct stat {
	uint16_t	st_dev;
	uint16_t	__pad1;
	unsigned long	st_ino;
	uint16_t	st_mode;
	uint16_t	st_nlink;
	uint16_t	st_uid;
	uint16_t	st_gid;
	uint16_t	st_rdev;
	uint16_t	__pad2;
	unsigned long	st_size;
	unsigned long	st_blksize;
	unsigned long	st_blocks;
	time_t		st_atime;
	unsigned long	__unused1;
	time_t		st_mtime;
	unsigned long	__unused2;
	time_t		st_ctime;
	unsigned long	__unused3;
	unsigned long	__unused4;
	unsigned long	__unused5;
};

struct stat64 {
	uint16_t	st_dev;
	unsigned char	__pad0[10];

#define STAT64_HAS_BROKEN_ST_INO	1
	unsigned long	__st_ino;

	uint32_t	st_mode;
	uint32_t	st_nlink;

	unsigned long	st_uid;
	unsigned long	st_gid;

	uint16_t	st_rdev;
	unsigned char	__pad3[10];

__extension__	long long	st_size __attribute__((__packed__));
	unsigned long	st_blksize;

	unsigned long	st_blocks;	/* Number 512-byte blocks allocated. */
	unsigned long	__pad4;		/* future possible st_blocks high bits */

	time_t		st_atime;
	unsigned long	__pad5;

	time_t		st_mtime;
	unsigned long	__pad6;

	time_t		st_ctime;
	unsigned long	__pad7;		/* will be high 32 bits of ctime someday */

__extension__	unsigned long long	st_ino __attribute__((__packed__));
};

#define S_IFMT  00170000
#define S_IFREG  0100000
#define S_IFBLK  0060000
#define S_IFDIR  0040000
#define S_IFCHR  0020000
#define S_IFIFO  0010000
#define S_ISUID  0004000
#define S_ISGID  0002000
#define S_ISVTX  0001000

#define S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)

#define S_IRWXU 00700
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100

#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001

extern int chmod(const char *_path, mode_t mode);
extern int fstat64(int fildes, struct stat64 *stat_buf);
extern int mkdir(const char *_path, mode_t mode);
extern int mkfifo(const char *_path, mode_t mode);
extern int stat64(const char *filename, struct stat64 *stat_buf);
extern mode_t umask(mode_t mask);

#endif
