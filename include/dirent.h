
#ifndef __DIRENT_H
#define __DIRENT_H

#include <linux/fs.h>
#include <stdint.h>

#define NAME_MAX NAME_LEN

struct dirent{
	long d_ino;
	off_t d_off;
	unsigned short d_reclen;
	char d_name[256];
};

struct dirent64 {
  uint64_t	d_ino;
  int64_t	d_off;
  uint16_t	d_reclen;
  unsigned char	d_type;
  char		d_name[256];
};

struct __DIR{
	struct dirent64 d_dent;
	int d_pos;
	int d_fd;
};

#define DIR struct __DIR 

DIR *opendir (const char *name);
int closedir (DIR *dir);
struct dirent *readdir (DIR *dir);

#endif 


