#ifdef __rtems__
#include <machine/rtems-bsd-user-space.h>
#include <machine/rtems-bsd-program.h>
#include "os/rtems/headers/rtems-bsd-fio-namespace.h"
#endif /* __rtems__ */

#include <string.h>

#include "log.h"
#include "pshared.h"

int cond_init_pshared(pthread_cond_t *cond)
{
	pthread_condattr_t cattr;
	int ret;

	ret = pthread_condattr_init(&cattr);
	if (ret) {
		log_err("pthread_condattr_init: %s\n", strerror(ret));
		return ret;
	}

#ifdef CONFIG_PSHARED
	ret = pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
	if (ret) {
		log_err("pthread_condattr_setpshared: %s\n", strerror(ret));
		return ret;
	}
#endif
	ret = pthread_cond_init(cond, &cattr);
	if (ret) {
		log_err("pthread_cond_init: %s\n", strerror(ret));
		return ret;
	}

	return 0;
}

int mutex_init_pshared(pthread_mutex_t *mutex)
{
	pthread_mutexattr_t mattr;
	int ret;

	ret = pthread_mutexattr_init(&mattr);
	if (ret) {
		log_err("pthread_mutexattr_init: %s\n", strerror(ret));
		return ret;
	}

	/*
	 * Not all platforms support process shared mutexes (FreeBSD)
	 */
#ifdef CONFIG_PSHARED
	ret = pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
	if (ret) {
		log_err("pthread_mutexattr_setpshared: %s\n", strerror(ret));
		return ret;
	}
#endif
	ret = pthread_mutex_init(mutex, &mattr);
	if (ret) {
		log_err("pthread_mutex_init: %s\n", strerror(ret));
		return ret;
	}

	return 0;
}

int mutex_cond_init_pshared(pthread_mutex_t *mutex, pthread_cond_t *cond)
{
	int ret;

	ret = mutex_init_pshared(mutex);
	if (ret)
		return ret;

	ret = cond_init_pshared(cond);
	if (ret)
		return ret;

	return 0;
}
#ifdef __rtems__
#include "os/rtems/headers/rtems-bsd-fio-pshared-data.h"
#endif /* __rtems__ */
