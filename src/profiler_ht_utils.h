#ifndef PROFILER_HT_UTILS
#define PROFILER_HT_UTILS

#include "types.h"
#include <string.h>

typedef struct profiler_ht_string_t {
    size_t length;
    size_t capacity;
    char* string;
    ui32 hash;
} profiler_ht_string_t;

size_t profiler_ht_hash(const char* string);

#endif