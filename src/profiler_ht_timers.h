#ifndef PROFILER_HT_TIMERS_H
#define PROFILER_HT_TIMERS_H

#include "profiler_ht_utils.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct profiler_ht_timers_entry_t {
    const char* name;
    ui64 calls_count;
    clock_t total_cycles;
} profiler_ht_timers_entry_t;

typedef struct profiler_ht_timers_entry_index_t {
    const char* name;
    ui64 index;
    struct profiler_ht_timers_entry_index_t* next;
} profiler_ht_timers_entry_index_t;

typedef struct profiler_ht_timers_t {
    profiler_ht_timers_entry_index_t* indices;
    profiler_ht_timers_entry_index_t* collisions;
    profiler_ht_timers_entry_t* entries;
    size_t indices_size;
    size_t indices_capacity;
    size_t collisions_size;
    size_t collisions_capacity;
    size_t entries_size;
    size_t entries_capacity;
} profiler_ht_timers_t;

profiler_ht_timers_t* profiler_ht_timers_init(size_t capacity);
void profiler_ht_timers_set(profiler_ht_timers_t* timers_table, const char* string);
void profiler_ht_timers_update(profiler_ht_timers_t* timers_table, const char* string, clock_t cycles);
void profiler_ht_timers_free(profiler_ht_timers_t* timers_table);

#endif