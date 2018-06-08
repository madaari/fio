#ifdef __rtems__
#include <machine/rtems-bsd-user-space.h>
#include <machine/rtems-bsd-program.h>
#include "os/rtems/headers/rtems-bsd-fio-namespace.h"
#include "parse.h"
#endif /* __rtems__ */

#include <stdlib.h>
#include "io_u_queue.h"

bool io_u_qinit(struct io_u_queue *q, unsigned int nr)
{
	q->io_us = calloc(nr, sizeof(struct io_u *));
	if (!q->io_us)
		return false;

	q->nr = 0;
	q->max = nr;
	return true;
}

void io_u_qexit(struct io_u_queue *q)
{
	free(q->io_us);
}

bool io_u_rinit(struct io_u_ring *ring, unsigned int nr)
{
	ring->max = nr + 1;
	if (ring->max & (ring->max - 1)) {
		ring->max--;
		ring->max |= ring->max >> 1;
		ring->max |= ring->max >> 2;
		ring->max |= ring->max >> 4;
		ring->max |= ring->max >> 8;
		ring->max |= ring->max >> 16;
		ring->max++;
	}

	ring->ring = calloc(ring->max, sizeof(struct io_u *));
	if (!ring->ring)
		return false;

	ring->head = ring->tail = 0;
	return true;
}

void io_u_rexit(struct io_u_ring *ring)
{
	free(ring->ring);
}
#ifdef __rtems__
#include "os/rtems/headers/rtems-bsd-fio-io_u_queue-data.h"
#endif /* __rtems__ */
