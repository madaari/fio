/*
 * fio - the flexible io tester
 *
 * Copyright (C) 2005 Jens Axboe <axboe@suse.de>
 * Copyright (C) 2006-2012 Jens Axboe <axboe@kernel.dk>
 *
 * The license below covers all files distributed with fio unless otherwise
 * noted in the file itself.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifdef __rtems__
#include <machine/rtems-bsd-user-space.h>
#include <machine/rtems-bsd-program.h>
#include "os/rtems/headers/rtems-bsd-fio-namespace.h"
#endif /* __rtems__ */

#include "fio.h"
static
int main(int argc, char *argv[], char *envp[])
{
	int ret = 1;

	compiletime_assert(TD_NR <= TD_ENG_FLAG_SHIFT, "TD_ENG_FLAG_SHIFT");

	if (initialize_fio(envp))
		return 1;

#if !defined(CONFIG_GETTIMEOFDAY) && !defined(CONFIG_CLOCK_GETTIME)
#error "No available clock source!"
#endif

	if (fio_server_create_sk_key())
		goto done;

	if (parse_options(argc, argv))
		goto done_key;

	/*
	 * line buffer stdout to avoid output lines from multiple
	 * threads getting mixed
	 */
	setvbuf(stdout, NULL, _IOLBF, 0);

	fio_time_init();

	if (nr_clients) {
		set_genesis_time();

		if (fio_start_all_clients())
			goto done_key;
		ret = fio_handle_clients(&fio_client_ops);
	} else
		ret = fio_backend(NULL);

done_key:
	fio_server_destroy_sk_key();
done:
	deinitialize_fio();
	return ret;
}

#ifdef __rtems__
#include "os/rtems/headers/rtems-bsd-fio-fio-data.h"
#endif /* __rtems__ */

RTEMS_LINKER_RWSET(bsd_prog_fio, char);

rtems_bsd_command_fio(int argc, char *argv[])
{
    int exit_code;
    void *data_begin;
    size_t data_size;

    data_begin = RTEMS_LINKER_SET_BEGIN(bsd_prog_fio);
    data_size = RTEMS_LINKER_SET_SIZE(bsd_prog_fio);

    rtems_bsd_program_lock();
    exit_code = rtems_bsd_program_call_main_with_data_restore("fio",
        main, argc, argv, data_begin, data_size);
    rtems_bsd_program_unlock();

    return exit_code;
}
