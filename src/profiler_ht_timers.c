#include "profiler_ht_timers.h"

static inline profiler_ht_timers_entry_t make_entry(const char* string);
static inline profiler_ht_timers_entry_index_t make_entry_index(const char* string, ui64 index);

static inline profiler_ht_timers_entry_t make_entry(const char* string) {
    return (profiler_ht_timers_entry_t){
        .name = string,
        .calls_count = 1,
        .total_cycles = 0
    };
}

static inline profiler_ht_timers_entry_index_t make_entry_index(const char* string, ui64 index) {
    return (profiler_ht_timers_entry_index_t){
        .name = string,
        .index = index,
        .next = NULL
    };
}

profiler_ht_timers_t* profiler_ht_timers_init(size_t capacity) {
    profiler_ht_timers_t* timers_table = (profiler_ht_timers_t*)malloc(sizeof(profiler_ht_timers_t));
    timers_table->indices = (profiler_ht_timers_entry_index_t*)calloc(capacity, sizeof(profiler_ht_timers_entry_index_t));
    timers_table->collisions = (profiler_ht_timers_entry_index_t*)malloc(sizeof(profiler_ht_timers_entry_index_t) * capacity);
    timers_table->entries = (profiler_ht_timers_entry_t*)malloc(sizeof(profiler_ht_timers_entry_t) * capacity);
    timers_table->indices_size = 0;
    timers_table->indices_capacity = capacity;
    timers_table->collisions_size = 0;
    timers_table->collisions_capacity = capacity;
    timers_table->entries_size = 0;
    timers_table->entries_capacity = capacity;
    return timers_table;
}

void profiler_ht_timers_set(profiler_ht_timers_t* timers_table, const char* string) {
    size_t index = profiler_ht_hash(string) & (timers_table->entries_capacity - 1);
    if(timers_table->indices[index].name == NULL) {
        timers_table->entries[timers_table->entries_size] = make_entry(string);
        timers_table->indices[index] = make_entry_index(string, timers_table->entries_size++);
        return;
    }
    profiler_ht_timers_entry_index_t* current_index = timers_table->indices + index;
    if(strcmp(string, current_index->name) == 0) {
        ++timers_table->entries[current_index->index].calls_count;
        return;
    }
    while(current_index->next != NULL) {
        if(strcmp(string, current_index->name) == 0) {
            ++timers_table->entries[current_index->index].calls_count;
            return;
        }
        current_index = current_index->next;
    }
    timers_table->entries[timers_table->entries_size] = make_entry(string);
    timers_table->collisions[timers_table->collisions_size] = make_entry_index(string, timers_table->entries_size++);
    current_index->next = timers_table->collisions + timers_table->collisions_size++;
}

void profiler_ht_timers_update(profiler_ht_timers_t* timers_table, const char* string, clock_t cycles) {
    size_t index = profiler_ht_hash(string) & (timers_table->entries_capacity - 1);
    profiler_ht_timers_entry_index_t* current_index = timers_table->indices + index;
    if(strcmp(string, current_index->name) == 0) {
        timers_table->entries[current_index->index].total_cycles += cycles;
        return;
    }
    while(current_index->next != NULL) {
        if(strcmp(string, current_index->name) == 0) {
            timers_table->entries[current_index->index].total_cycles += cycles;
            return;
        }
        current_index = current_index->next;
    }
}

void profiler_ht_timers_free(profiler_ht_timers_t* timers_table) {
    free(timers_table->indices);
    free(timers_table->collisions);
    free(timers_table->entries);
    free(timers_table);
}