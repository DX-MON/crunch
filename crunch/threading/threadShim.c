#include "threadShim.h"
#include <errno.h>
#include <stdint.h>

typedef void *(*pthread_start_t)(void *);

int thrd_err_map(const int result)
{
	switch (result)
	{
		case 0:
			return thrd_success;
		case ENOMEM:
			return thrd_nomem;
		case ETIMEDOUT:
			return thrd_timedout;
		case EBUSY:
			return thrd_busy;
		default:
			return thrd_error;
	}
}

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg)
{
	pthread_attr_t threadAttrs;
	pthread_attr_init(&threadAttrs);
	pthread_attr_setdetachstate(&threadAttrs, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setscope(&threadAttrs, PTHREAD_SCOPE_PROCESS);
	const int result = pthread_create(thr, &threadAttrs, (pthread_start_t)(void *)func, arg);
	pthread_attr_destroy(&threadAttrs);
	return thrd_err_map(result);
}

int thrd_join(thrd_t thr, int *res)
{
	void *resultPtr = NULL;
	const int result = pthread_join(thr, &resultPtr);
	if (res)
		*res = (int)(uintptr_t)resultPtr;
	return thrd_err_map(result);
}

void thrd_exit(int res) { pthread_exit((void *)(uintptr_t)res); }