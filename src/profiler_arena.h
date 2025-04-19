#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>
#include "types.h"

typedef struct profiler_arena_t {
    char* memory;
    char* start;
    char* current;
    char* end;
    struct profiler_arena_t* next;
    ui64 capacity;
} profiler_arena_t;

profiler_arena_t* arena_alloc(ui64 capacity);
char* arena_get_memory(profiler_arena_t* arena, ui64 capacity);
void arena_free(profiler_arena_t* arena);

#endif