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
#include <sys/resource.h>
#include <stdatomic.h>
//#include "rtems/sys/ipc.h"
#include "../file.h"

#define FIO_HAVE_ODIRECT
#define FIO_USE_GENERIC_RAND
#define FIO_USE_GENERIC_INIT_RANDOM_STATE
#define off64_t _off64_t
#define OS_MAP_ANON		MAP_ANON
#define O_DIRECT	0x1000000
#define FIO_NO_HAVE_SHM_H
#define CONFIG_NO_SHM
#define CONFIG_HAVE_MKDIR_TWO
#define SA_RESTART	0

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
