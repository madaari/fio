#ifndef ARCH_X86_H
#define ARCH_X86_H

#define ARCH	(arch_i386)

#ifndef __NR_ioprio_set
#define __NR_ioprio_set		289
#define __NR_ioprio_get		290
#endif

#ifndef __NR_fadvise64
#define __NR_fadvise64		250
#endif

#ifndef __NR_sys_splice
#define __NR_sys_splice		313
#define __NR_sys_tee		315
#define __NR_sys_vmsplice	316
#endif

#ifndef __NR_async_exec
#define __NR_async_exec		320
#define __NR_async_wait		321
#define __NR_umem_add		322
#define __NR_async_thread	323
#endif

#define	FIO_HUGE_PAGE		4194304

#define FIO_HAVE_SYSLET

#define nop	__asm__ __volatile__("rep;nop": : :"memory")

static inline unsigned long fio_ffz(unsigned long bitmask)
{
	__asm__("bsfl %1,%0" :"=r" (bitmask) :"r" (~bitmask));
	return bitmask;
}

#endif
