#ifndef _SYS_RESOURCE_H_
#define _SYS_RESOURCE_H_

#define RUSAGE_SELF	0
#define RUSAGE_THREAD	1

struct rusage
{
	struct timeval ru_utime;
	struct timeval ru_stime;
	int ru_nvcsw;
	int ru_minflt;
	int ru_majflt;
	int ru_nivcsw;
};

int getrusage(int who, struct rusage *r_usage);

#endif /* SYS_RESOURCE_H */
