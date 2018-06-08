#ifdef __rtems__
#include <machine/rtems-bsd-user-space.h>
#include <machine/rtems-bsd-program.h>
#include "../os/rtems/headers/rtems-bsd-fio-namespace.h"
#include "parse.h"
#endif /* __rtems__ */

#include "fnv.h"

#define FNV_PRIME	0x100000001b3ULL

/*
 * 64-bit fnv, but don't require 64-bit multiples of data. Use bytes
 * for the last unaligned chunk.
 */
uint64_t fnv(const void *buf, uint32_t len, uint64_t hval)
{
	const uint64_t *ptr = buf;

	while (len) {
		hval *= FNV_PRIME;
		if (len >= sizeof(uint64_t)) {
			hval ^= (uint64_t) *ptr++;
			len -= sizeof(uint64_t);
			continue;
		} else {
			const uint8_t *ptr8 = (const uint8_t *) ptr;
			uint64_t val = 0;
			int i;

			for (i = 0; i < len; i++) {
				val <<= 8;
				val |= (uint8_t) *ptr8++;
			}
			hval ^= val;
			break;
		}
	}

	return hval;
}
#ifdef __rtems__
#include "../os/rtems/headers/rtems-bsd-fio-fnv-data.h"
#endif /* __rtems__ */
