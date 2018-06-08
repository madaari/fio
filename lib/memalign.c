#ifdef __rtems__
#include <machine/rtems-bsd-user-space.h>
#include <machine/rtems-bsd-program.h>
#include "../os/rtems/headers/rtems-bsd-fio-namespace.h"
#include "parse.h"
#endif /* __rtems__ */

#include <assert.h>
#include <stdlib.h>

#include "memalign.h"

#define PTR_ALIGN(ptr, mask)   \
	(char *)((uintptr_t)((ptr) + (mask)) & ~(mask))

struct align_footer {
	unsigned int offset;
};

void *fio_memalign(size_t alignment, size_t size)
{
	struct align_footer *f;
	void *ptr, *ret = NULL;

	assert(!(alignment & (alignment - 1)));

	ptr = malloc(size + alignment + sizeof(*f) - 1);
	if (ptr) {
		ret = PTR_ALIGN(ptr, alignment - 1);
		f = ret + size;
		f->offset = (uintptr_t) ret - (uintptr_t) ptr;
	}

	return ret;
}

void fio_memfree(void *ptr, size_t size)
{
	struct align_footer *f = ptr + size;

	free(ptr - f->offset);
}
#ifdef __rtems__
#include "../os/rtems/headers/rtems-bsd-fio-memalign-data.h"
#endif /* __rtems__ */
