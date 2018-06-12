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
    puts("calling destructors\n");
    fio_syncio_unregister();
    tiobench_unregister();
    fio_cpuio_unregister();
    fio_filecreate_unregister();
    fio_mmapio_unregister();
    fio_null_unregister();
    act_unregister();
    fio_netio_unregister();
	return ret;
}

#ifdef __rtems__
#include "os/rtems/headers/rtems-bsd-fio-fio-data.h"

void act_register(void);
void tiobench_register(void);
void fio_syncio_register(void);
void fio_cpuio_register(void);
void fio_filecreate_register(void);
void fio_gf_register(void);
void fio_mmapio_register(void);
void fio_null_register(void);
void fio_netio_register(void);
void prio_tree_init(void);
void fio_syncio_register_ft(void);
void fio_client_hash_init(void);
void fio_syncio_unregister(void);
void tiobench_unregister(void);
void fio_cpuio_unregister(void);
void fio_filecreate_unregister(void);
void fio_mmapio_unregister(void);
void fio_null_unregister(void);
void act_unregister(void);
void fio_netio_unregister(void);

static int
mainwrapper(int argc, char *argv[])
{
	act_register();
	tiobench_register();
	fio_syncio_register();
	fio_cpuio_register();
	fio_filecreate_register();
#ifdef CONFIG_GFAPI
	fio_gf_register();
#endif
	fio_mmapio_register();
	fio_null_register();
	fio_netio_register();
	prio_tree_init();
	fio_syncio_register_ft();
	fio_client_hash_init();
	printf("Constructors called\n");

	return main(argc, argv, (char *)NULL);
}

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
        mainwrapper, argc, argv, data_begin, data_size);
        
     rtems_bsd_program_unlock();
            
    return exit_code;
}
#endif /* __rtems__ */
