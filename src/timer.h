#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <time.h>
#include "types.h"

typedef struct timer_t {
    const char* name;
    clock_t start;
    clock_t end;
    ui64 trace_index;
} timer_t;

#endif