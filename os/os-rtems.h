/*-
 * Copyright (c) 2018 Udit kumar agarwal <dev.madaari@gmail.com>
 * All rights reserved.
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

#ifndef FIO_OS_RTEMS_H
#define FIO_OS_RTEMS_H

#define	FIO_OS	os_rtems

#include <errno.h>
#include <machine/endian.h>
#include <sys/socket.h>
#include <machine/param.h>
#include <sys/cpuset.h>
#include <sys/resource.h>
#include <stdatomic.h>
#include "../file.h"

#define FIO_HAVE_ODIRECT
#define FIO_USE_GENERIC_RAND
#define FIO_USE_GENERIC_INIT_RANDOM_STATE
#define off64_t _off64_t
#define OS_MAP_ANON		MAP_ANON
#define O_DIRECT	0x1000000
#define FIO_NO_HAVE_SHM_H
#define CONFIG_NO_SHM
#define CONFIG_HAVE_MKDIR_TWO
#define SA_RESTART	0
#define __need_getopt_newlib

static inline int blockdev_invalidate_cache(struct fio_file *f)
{
	return ENOTSUP;
}

static inline int blockdev_size(struct fio_file *f, unsigned long long *bytes)
{
	errno = ENOSYS;
	return 0;
}

static inline unsigned long long os_phys_mem(void)
{
	errno = ENOSYS;
	return 0;
}

#endif
