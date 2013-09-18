extern int sys_setup();
extern int sys_exit();
extern int sys_fork();
extern int sys_read();
extern int sys_write();
extern int sys_open();
extern int sys_close();
extern int sys_waitpid();
extern int sys_creat();
extern int sys_link();
extern int sys_unlink();
extern int sys_execve();
extern int sys_chdir();
extern int sys_time();
extern int sys_mknod();
extern int sys_chmod();
extern int sys_chown();
extern int sys_break();
extern int sys_oldstat();
extern int sys_lseek();
extern int sys_getpid();
extern int sys_mount();
extern int sys_umount();
extern int sys_setuid();
extern int sys_getuid();
extern int sys_stime();
extern int sys_ptrace();
extern int sys_alarm();
extern int sys_oldfstat();
extern int sys_pause();
extern int sys_utime();
extern int sys_stty();
extern int sys_gtty();
extern int sys_access();
extern int sys_nice();
extern int sys_ftime();
extern int sys_sync();
extern int sys_kill();
extern int sys_rename();
extern int sys_mkdir();
extern int sys_rmdir();
extern int sys_dup();
extern int sys_pipe();
extern int sys_times();
extern int sys_prof();
extern int sys_brk();
extern int sys_setgid();
extern int sys_getgid();
extern int sys_signal();
extern int sys_geteuid();
extern int sys_getegid();
extern int sys_acct();
extern int sys_phys();
extern int sys_lock();
extern int sys_ioctl();
extern int sys_fcntl();
extern int sys_mpx();
extern int sys_setpgid();
extern int sys_ulimit();
extern int sys_oldolduname();
extern int sys_umask();
extern int sys_chroot();
extern int sys_ustat();
extern int sys_dup2();
extern int sys_getppid();
extern int sys_getpgrp();
extern int sys_setsid();
extern int sys_stat();
extern int sys_lstat();
extern int sys_fstat();
extern int sys_uname();
extern int sys_getdents();
extern int sys_stat64();
extern int sys_lstat64();
extern int sys_fstat64();
extern int sys_getdents64();
extern int sys_fcntl64();
extern int sys_null();	/* not implemented sys_call */

fn_ptr sys_call_table[] = { sys_setup, sys_exit, sys_fork, sys_read,
sys_write, sys_open, sys_close, sys_waitpid, sys_creat, sys_link,
sys_unlink, sys_execve, sys_chdir, sys_time, sys_mknod, sys_chmod,
sys_chown, sys_break, sys_oldstat, sys_lseek, sys_getpid, sys_mount,
sys_umount, sys_setuid, sys_getuid, sys_stime, sys_ptrace, sys_alarm,
sys_oldfstat, sys_pause, sys_utime, sys_stty, sys_gtty, sys_access,
sys_nice, sys_ftime, sys_sync, sys_kill, sys_rename, sys_mkdir,
sys_rmdir, sys_dup, sys_pipe, sys_times, sys_prof, sys_brk, sys_setgid,
sys_getgid, sys_signal, sys_geteuid, sys_getegid, sys_acct, sys_phys,
sys_lock, sys_ioctl, sys_fcntl, sys_mpx, sys_setpgid, sys_ulimit,
sys_oldolduname, sys_umask, sys_chroot, sys_ustat, sys_dup2, sys_getppid,
sys_getpgrp,sys_setsid, sys_null /*67*/,sys_null,sys_null,

/* 70 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 80 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 90 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 100 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_stat,sys_lstat,sys_fstat,sys_null,
/* 110 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 120 */sys_null,sys_null,sys_uname,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 130 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 140 */sys_null,sys_getdents,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 150 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 160 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 170 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 180 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 190 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_stat64,sys_lstat64,sys_fstat64,sys_null,sys_null,
/* 200 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 210 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 220 */sys_getdents64,sys_fcntl64,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 230 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 240 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 250 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 260 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 270 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 280 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 290 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 300 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null,
/* 310 */sys_null,sys_null,sys_null,sys_null,sys_null,
sys_null,sys_null,sys_null,sys_null,sys_null
};

