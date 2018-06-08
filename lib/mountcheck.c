#ifdef __rtems__
#include <machine/rtems-bsd-user-space.h>
#include <machine/rtems-bsd-program.h>
#include "../os/rtems/headers/rtems-bsd-fio-namespace.h"
#endif /* __rtems__ */

#include <stdio.h>
#include <string.h>

#ifdef CONFIG_GETMNTENT
#include <mntent.h>

#include "mountcheck.h"

#define MTAB	"/etc/mtab"

int device_is_mounted(const char *dev)
{
	FILE *mtab;
	struct mntent *mnt;
	int ret = 0;

	mtab = setmntent(MTAB, "r");
	if (!mtab)
		return 0;

	while ((mnt = getmntent(mtab)) != NULL) {
		if (!mnt->mnt_fsname)
			continue;
		if (!strcmp(mnt->mnt_fsname, dev)) {
			ret = 1;
			break;
		}
	}

	endmntent(mtab);
	return ret;
}

#elif defined(CONFIG_GETMNTINFO)
/* for most BSDs */
#include <sys/param.h>
#include <sys/mount.h>

int device_is_mounted(const char *dev)
{
	struct statfs *st;
	int i, ret;

	ret = getmntinfo(&st, MNT_NOWAIT);
	if (ret <= 0)
		return 0;

	for (i = 0; i < ret; i++) {
		if (!strcmp(st[i].f_mntfromname, dev))
			return 1;
	}

	return 0;
}

#elif defined(CONFIG_GETMNTINFO_STATVFS)
/* for NetBSD */
#include <sys/statvfs.h>

int device_is_mounted(const char *dev)
{
	struct statvfs *st;
	int i, ret;

	ret = getmntinfo(&st, MNT_NOWAIT);
	if (ret <= 0)
		return 0;

	for (i = 0; i < ret; i++) {
		if (!strcmp(st[i].f_mntfromname, dev))
			return 1;
	}

	return 0;
}

#else
/* others */

int device_is_mounted(const char *dev)
{
	return 0;
}

#endif
#ifdef __rtems__
#include "../os/rtems/headers/rtems-bsd-fio-mountcheck-data.h"
#endif /* __rtems__ */
