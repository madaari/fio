#ifndef FIO_OS_RTEMS_H
#define FIO_OS_RTEMS_H

#define	FIO_OS	os_rtems

#include <errno.h>
//#include <sys/sysctl.h>
//#include <sys/disk.h>
#include <machine/endian.h>
//#include <sys/thr.h>
#include <sys/socket.h>
#include <machine/param.h>
#include <sys/cpuset.h>
//#include <sys/statvfs.h>

#include <sys/file.h>

#define FIO_HAVE_ODIRECT
#define FIO_USE_GENERIC_RAND
#define FIO_USE_GENERIC_INIT_RANDOM_STATE
#define CTL_HW 6

#define OS_MAP_ANON		MAP_ANON


static inline int blockdev_size(struct fio_file *f, unsigned long long *bytes)
{
	errno = ENOSYS;
	return errno;
}


static inline int blockdev_invalidate_cache(struct fio_file *f)
{
	return ENOTSUP;
}

static inline unsigned long long os_phys_mem(void)
{
	errno = ENOSYS;
	return errno;
}



#endif
