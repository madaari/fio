#ifdef __rtems__
#include <machine/rtems-bsd-user-space.h>
#include <machine/rtems-bsd-program.h>
#include "os/rtems/headers/rtems-bsd-fio-namespace.h"
#endif /* __rtems__ */

#include <assert.h>
#include <stdarg.h>

#include "debug.h"
#include "log.h"

#ifdef FIO_INC_DEBUG
void __dprint(int type, const char *str, ...)
{
	va_list args;

	assert(type < FD_DEBUG_MAX);

	va_start(args, str);
	log_prevalist(type, str, args);
	va_end(args);
}
#endif
#ifdef __rtems__
#include "os/rtems/headers/rtems-bsd-fio-debug-data.h"
#endif /* __rtems__ */
