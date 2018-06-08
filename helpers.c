#ifdef __rtems__
#include <machine/rtems-bsd-user-space.h>
#include <machine/rtems-bsd-program.h>
#include "os/rtems/headers/rtems-bsd-fio-namespace.h"
#include "parse.h"
#endif /* __rtems__ */

#include <errno.h>

#include "helpers.h"

#ifndef CONFIG_LINUX_FALLOCATE
int fallocate(int fd, int mode, off_t offset, off_t len)
{
	errno = ENOSYS;
	return -1;
}
#endif

#ifndef CONFIG_POSIX_FALLOCATE
int posix_fallocate(int fd, off_t offset, off_t len)
{
	return 0;
}
#endif

#ifndef CONFIG_SYNC_FILE_RANGE
int sync_file_range(int fd, off64_t offset, off64_t nbytes, unsigned int flags)
{
	errno = ENOSYS;
	return -1;
}
#endif

#ifndef CONFIG_POSIX_FADVISE
int posix_fadvise(int fd, off_t offset, off_t len, int advice)
{
	return 0;
}
#endif
#ifdef __rtems__
#include "os/rtems/headers/rtems-bsd-fio-helpers-data.h"
#endif /* __rtems__ */
