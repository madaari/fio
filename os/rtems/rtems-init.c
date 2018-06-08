/*
 * Copyright (c) 2010-2016 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifdef __rtems__
#include <machine/rtems-bsd-user-space.h>
#include <machine/rtems-bsd-program.h>
#include "os/rtems/headers/rtems-bsd-fio-namespace.h"
#endif /* __rtems__ */

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <sysexits.h>

#include <machine/rtems-bsd-commands.h>

#include <net/if.h>

#include <rtems/bdbuf.h>
#include <rtems/console.h>
#include <rtems/ftpd.h>
#include <rtems/media.h>
#include <rtems/shell.h>
#include <rtems/telnetd.h>
#include <rtems.h>
#include <rtems/printer.h>
#include <rtems/stackchk.h>
#include <rtems/bsd/bsd.h>
#include <rtems/bsd/modules.h>
#include <rtems/dhcpcd.h>
#include <rtems/console.h>
#include <rtems/shell.h>
#include <rtems/linkersets.h>
#include "headers/rtems-bsd-fio-data.h"

int main(int argc, char *argv[]);

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

#define DEFAULT_NETWORK_SHELL

static rtems_status_code
media_listener(rtems_media_event event, rtems_media_state state,
    const char *src, const char *dest, void *arg)
{
	
	if (dest != NULL) {
		printf(", dest = %s", dest);
	}

	if (arg != NULL) {
		printf(", arg = %p\n", arg);
	}

	printf("\n");

	if (event == RTEMS_MEDIA_EVENT_MOUNT && state == RTEMS_MEDIA_STATE_SUCCESS) {
		printf("MEDIA MOUNTED");
	}

	return RTEMS_SUCCESSFUL;
}

rtems_shell_cmd_t rtems_shell_fio_Command = {
    .name = "fio",
    .usage = "fio",
    .topic = "user",
    .command = rtems_bsd_command_fio
};

static void
early_initialization(void)
{
	rtems_status_code sc;

	sc = rtems_bdbuf_init();
	assert(sc == RTEMS_SUCCESSFUL);

	sc = rtems_media_initialize();
	assert(sc == RTEMS_SUCCESSFUL);

	sc = rtems_media_listener_add(media_listener, NULL);
	assert(sc == RTEMS_SUCCESSFUL);

	sc = rtems_media_server_initialize(
		200,
		32 * 1024,
		RTEMS_DEFAULT_MODES,
		RTEMS_DEFAULT_ATTRIBUTES
	);
	assert(sc == RTEMS_SUCCESSFUL);
}
void
Init(rtems_task_argument arg)
{
	rtems_status_code sc;
	
	puts("FIO TEST");

	early_initialization();
	rtems_bsd_initialize();

	/* Let the callout timer allocate its resources */
	sc = rtems_task_wake_after(2);
	assert(sc == RTEMS_SUCCESSFUL);

	sc = rtems_shell_init("SHLL", 16 * 1024, 1, CONSOLE_DEVICE_NAME,
	    false, true, NULL);
	assert(sc == RTEMS_SUCCESSFUL);

	assert(0);
}


#define DEFAULT_NETWORK_DHCPCD_ENABLE

#define CONFIGURE_MICROSECONDS_PER_TICK 1000

#define CONFIGURE_MAXIMUM_DRIVERS 32

#define CONFIGURE_FILESYSTEM_DOSFS

#define CONFIGURE_MAXIMUM_PROCESSORS 32

#if defined(DEFAULT_NETWORK_DHCPCD_ENABLE) && \
    !defined(DEFAULT_NETWORK_NO_STATIC_IFCONFIG)
#define DEFAULT_NETWORK_NO_STATIC_IFCONFIG
#endif

#ifndef DEFAULT_NETWORK_NO_STATIC_IFCONFIG
#include <rtems/bsd/test/network-config.h>
#endif
/*
 * Configure LibBSD.
 */

#if defined(LIBBSP_I386_PC386_BSP_H)
#define RTEMS_BSD_CONFIG_DOMAIN_PAGE_MBUFS_SIZE (64 * 1024 * 1024)
#elif defined(LIBBSP_POWERPC_QORIQ_BSP_H)
#define RTEMS_BSD_CONFIG_DOMAIN_PAGE_MBUFS_SIZE (32 * 1024 * 1024)
#endif

#define RTEMS_BSD_CONFIG_NET_PF_UNIX
#define RTEMS_BSD_CONFIG_NET_IP_MROUTE
#define RTEMS_BSD_CONFIG_NET_IP6_MROUTE
#define RTEMS_BSD_CONFIG_NET_IF_BRIDGE
#define RTEMS_BSD_CONFIG_NET_IF_LAGG
#define RTEMS_BSD_CONFIG_NET_IF_VLAN
#define RTEMS_BSD_CONFIG_BSP_CONFIG
#define RTEMS_BSD_CONFIG_INIT

#include <machine/rtems-bsd-config.h>

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_STUB_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_ZERO_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 32

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS 1

#define CONFIGURE_UNLIMITED_ALLOCATION_SIZE 32
#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS

#define CONFIGURE_STACK_CHECKER_ENABLED

#define CONFIGURE_BDBUF_BUFFER_MAX_SIZE (64 * 1024)
#define CONFIGURE_BDBUF_MAX_READ_AHEAD_BLOCKS 4
#define CONFIGURE_BDBUF_CACHE_MEMORY_SIZE (1 * 1024 * 1024)

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT_TASK_STACK_SIZE (128 * 1024)
#define CONFIGURE_INIT_TASK_INITIAL_MODES RTEMS_DEFAULT_MODES
#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT

#define CONFIGURE_INIT
#define CPU_STACK_MINIMUM_SIZE           (128*1024)
#include <rtems/confdefs.h>

#ifdef DEFAULT_NETWORK_SHELL

#define CONFIGURE_SHELL_COMMANDS_INIT

#include <bsp/irq-info.h>

#include <rtems/netcmds-config.h>

#ifdef RTEMS_BSD_MODULE_USER_SPACE_WLANSTATS
  #define SHELL_WLANSTATS_COMMAND &rtems_shell_WLANSTATS_Command,
#else
  #define SHELL_WLANSTATS_COMMAND
#endif

#ifdef RTEMS_BSD_MODULE_USR_SBIN_WPA_SUPPLICANT
  #define SHELL_WPA_SUPPLICANT_COMMAND &rtems_shell_WPA_SUPPLICANT_Command,
#else
  #define SHELL_WPA_SUPPLICANT_COMMAND
#endif

#define CONFIGURE_SHELL_USER_COMMANDS \
  SHELL_WLANSTATS_COMMAND \
  SHELL_WPA_SUPPLICANT_COMMAND \
  &bsp_interrupt_shell_command, \
  &rtems_shell_ARP_Command, \
  &rtems_shell_HOSTNAME_Command, \
  &rtems_shell_PING_Command, \
  &rtems_shell_ROUTE_Command, \
  &rtems_shell_NETSTAT_Command, \
  &rtems_shell_IFCONFIG_Command, \
  &rtems_shell_TCPDUMP_Command, \
  &rtems_shell_SYSCTL_Command, \
  &rtems_shell_VMSTAT_Command, \
  &rtems_shell_fio_Command

#define CONFIGURE_SHELL_COMMAND_CPUINFO
#define CONFIGURE_SHELL_COMMAND_CPUUSE
#define CONFIGURE_SHELL_COMMAND_PERIODUSE
#define CONFIGURE_SHELL_COMMAND_STACKUSE
#define CONFIGURE_SHELL_COMMAND_PROFREPORT

#define CONFIGURE_SHELL_COMMAND_CP
#define CONFIGURE_SHELL_COMMAND_PWD
#define CONFIGURE_SHELL_COMMAND_LS
#define CONFIGURE_SHELL_COMMAND_LN
#define CONFIGURE_SHELL_COMMAND_LSOF
#define CONFIGURE_SHELL_COMMAND_CHDIR
#define CONFIGURE_SHELL_COMMAND_CD
#define CONFIGURE_SHELL_COMMAND_MKDIR
#define CONFIGURE_SHELL_COMMAND_RMDIR
#define CONFIGURE_SHELL_COMMAND_CAT
#define CONFIGURE_SHELL_COMMAND_MV
#define CONFIGURE_SHELL_COMMAND_RM
#define CONFIGURE_SHELL_COMMAND_MALLOC_INFO
#define CONFIGURE_SHELL_COMMAND_SHUTDOWN

#endif /* DEFAULT_NETWORK_SHELL */


#ifdef RTEMS_BSD_MODULE_USR_SBIN_WPA_SUPPLICANT
  #define SHELL_WPA_SUPPLICANT_COMMANDS \
    &rtems_shell_WPA_SUPPLICANT_Command, \
    &rtems_shell_WPA_SUPPLICANT_FORK_Command,
#else
  #define SHELL_WPA_SUPPLICANT_COMMANDS
#endif

#define CONFIGURE_SHELL_COMMAND_FDISK
#define CONFIGURE_SHELL_COMMAND_BLKSTATS
#define CONFIGURE_SHELL_COMMAND_BLKSYNC
#define CONFIGURE_SHELL_COMMAND_MSDOSFMT
#define CONFIGURE_SHELL_COMMAND_DF
#define CONFIGURE_SHELL_COMMAND_MOUNT
#define CONFIGURE_SHELL_COMMAND_UNMOUNT
#define CONFIGURE_SHELL_COMMAND_MSDOSFMT
#define CONFIGURE_SHELL_COMMAND_EDIT
#define CONFIGURE_SHELL_COMMAND_GETENV
#define CONFIGURE_SHELL_COMMAND_SETENV
#define CONFIGURE_SHELL_COMMAND_UNSETENV
#include <rtems/shellconfig.h>
