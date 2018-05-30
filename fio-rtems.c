/*
 * RTEMS specific fio main file
 *
 * Copyright (C) 2005 Jens Axboe <axboe@suse.de>
 * Copyright (C) 2006-2012 Jens Axboe <axboe@kernel.dk>
 * Copyright (C) 2018 Christian Mauderer <christian.mauderer@embedded-brains.de>
 * Copyright (C) 2018 Udit Kumar Agarwal <dev.madaari@gmail.com>
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

#include "fio.h"

#include <stdlib.h>
#include <rtems.h>
#include <bsp.h>
#include <rtems/shell.h>

#define PRIO_DHCP		(RTEMS_MAXIMUM_PRIORITY - 1)
#define PRIO_WPA		(RTEMS_MAXIMUM_PRIORITY - 1)
#define PRIO_INIT_TASK		(RTEMS_MAXIMUM_PRIORITY - 1)
#define PRIO_MEDIA_SERVER	200

int fio_main(int argc, char *argv[], char *envp[])
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

static void
Init(rtems_task_argument arg)
{

	puts("\nRTEMS FIO \n");
	rtems_shell_env_t env;

    memset(&env, 0, sizeof(env));
    rtems_shell_main_loop(&env);
	exit(0);
}

rtems_shell_cmd_t rtems_shell_fio_Command = {
    .name = "fio",
    .usage = "fio",
    .topic = "user",
    .command = fio_main
};

/*
 * Configure LibBSD.
 */
#define RTEMS_BSD_CONFIG_BSP_CONFIG
#define RTEMS_BSD_CONFIG_TERMIOS_KQUEUE_AND_POLL
#define RTEMS_BSD_CONFIG_INIT

#include <machine/rtems-bsd-config.h>

/*
 * Configure RTEMS.
 */
#define CONFIGURE_MICROSECONDS_PER_TICK 1000

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_STUB_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_ZERO_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK

#define CONFIGURE_FILESYSTEM_DOSFS
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 32

#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS
#define CONFIGURE_MAXIMUM_USER_EXTENSIONS 1

#define CONFIGURE_INIT_TASK_STACK_SIZE (64*1024)
#define CONFIGURE_INIT_TASK_INITIAL_MODES RTEMS_DEFAULT_MODES
#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT

#define CONFIGURE_BDBUF_BUFFER_MAX_SIZE (32 * 1024)
#define CONFIGURE_BDBUF_MAX_READ_AHEAD_BLOCKS 4
#define CONFIGURE_BDBUF_CACHE_MEMORY_SIZE (1 * 1024 * 1024)
#define CONFIGURE_BDBUF_READ_AHEAD_TASK_PRIORITY 97
#define CONFIGURE_SWAPOUT_TASK_PRIORITY 97

//#define CONFIGURE_STACK_CHECKER_ENABLED

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INIT

#include <rtems/confdefs.h>

/*
 * Configure Shell.
 */
#include <rtems/netcmds-config.h>
#include <bsp/irq-info.h>
#define CONFIGURE_SHELL_COMMANDS_INIT

#define CONFIGURE_SHELL_USER_COMMANDS \
  &rtems_shell_fio_Command

#define CONFIGURE_SHELL_COMMANDS_ALL

#include <rtems/shellconfig.h>
