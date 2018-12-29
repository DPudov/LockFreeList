#ifndef __BENCHMARK_H__
#define __BENCHMARK_H__

#include "list.h"
#include "types.h"
#include <pthread.h>

#define THREAD_COUNT 40
#define THREAD_STACK_SIZE 4092
#define INSERTS 10000

#define RETURN_TIME (((unsigned long long) high << 32) | low)

#endif
