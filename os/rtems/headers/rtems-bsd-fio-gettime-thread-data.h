/* generated by userspace-header-gen.py */
#include <rtems/linkersets.h>
#include "rtems-bsd-fio-data.h"
/* gettime-thread.c */
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_fio, static pthread_t gtod_thread);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_fio, static os_cpu_mask_t fio_gtod_cpumask);
