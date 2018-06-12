/*
 * ftruncate: ioengine for git://git.kernel.dk/fio.git
 *
 * IO engine that does regular truncates to simulate data transfer
 * as fio ioengine.
 * DDIR_WRITE does ftruncate
 *
 */
#ifdef __rtems__
#include <machine/rtems-bsd-user-space.h>
#include <machine/rtems-bsd-program.h>
#include "../os/rtems/headers/rtems-bsd-fio-namespace.h"
#endif /* __rtems__ */

#include <errno.h>
#include <unistd.h>

#include "../fio.h"

static enum fio_q_status fio_ftruncate_queue(struct thread_data *td,
					     struct io_u *io_u)
{
	struct fio_file *f = io_u->file;
	int ret;

	fio_ro_check(td, io_u);

	if (io_u->ddir != DDIR_WRITE) {
		io_u->error = EINVAL;
		return FIO_Q_COMPLETED;
	}

	ret = ftruncate(f->fd, io_u->offset);
	if (ret)
		io_u->error = errno;

	return FIO_Q_COMPLETED;
}

static struct ioengine_ops ioengine = {
	.name		= "ftruncate",
	.version	= FIO_IOOPS_VERSION,
	.queue		= fio_ftruncate_queue,
	.open_file	= generic_open_file,
	.close_file	= generic_close_file,
	.get_file_size	= generic_get_file_size,
	.flags		= FIO_SYNCIO | FIO_FAKEIO
};
#ifdef __rtems__
void fio_syncio_register_ft(void)
#else
static void fio_init fio_syncio_register(void)
#endif
{
	register_ioengine(&ioengine);
}

#ifdef __rtems__
void
#else
static void fio_exit
#endif
fio_syncio_unregister(void)
{
	unregister_ioengine(&ioengine);
}
#ifdef __rtems__
#include "../os/rtems/headers/rtems-bsd-fio-ftruncate-data.h"
#endif /* __rtems__ */
