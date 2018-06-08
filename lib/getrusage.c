#ifdef __rtems__
#include <machine/rtems-bsd-user-space.h>
#include <machine/rtems-bsd-program.h>
#include "../os/rtems/headers/rtems-bsd-fio-namespace.h"
#endif /* __rtems__ */

#include <errno.h>
#include "getrusage.h"

int fio_getrusage(struct rusage *ru)
{
#ifdef CONFIG_RUSAGE_THREAD
	if (!getrusage(RUSAGE_THREAD, ru))
		return 0;
	if (errno != EINVAL)
		return -1;
	/* Fall through to RUSAGE_SELF */
#endif
	return getrusage(RUSAGE_SELF, ru);
}
#ifdef __rtems__
#include "../os/rtems/headers/rtems-bsd-fio-getrusage-data.h"
#endif /* __rtems__ */
