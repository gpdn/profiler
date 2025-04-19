#include "profiler_arena.h"

profiler_arena_t* arena_alloc(ui64 capacity) {
    profiler_arena_t* arena = (profiler_arena_t*)malloc(sizeof(profiler_arena_t));
    arena->memory = malloc(capacity);
    arena->start = arena->memory;
    arena->current = arena->memory;
    arena->end = arena->start + capacity;
    arena->capacity = capacity;
    return arena;
}

char* arena_get_memory(profiler_arena_t* arena, ui64 capacity) {
    if((ui64)(arena->end - arena->current) < capacity) {
        arena->next = arena_alloc(arena->capacity + capacity);
        arena->start = arena->next->start;
        arena->current = arena->next->start;
        arena->end = arena->next->end;
        return arena->current;
    }

    char* start = arena->current;
    arena->current += capacity;
    return start;
}

void arena_free(profiler_arena_t* arena) {
    free(arena->memory);
    if(arena->next) arena_free(arena->next);
}