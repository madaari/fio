#include "inet_aton.h"
#ifndef __rtems__
int inet_aton(const char *cp, struct in_addr *inp)
{
	return inet_pton(AF_INET, cp, inp);
}
#endif
