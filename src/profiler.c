#include "profiler.h"
#include <string.h>

#define PROFILER_DEFAUT_MEMORY 4096
#define PROFILER_INDENT_STRING_SIZE 255

static inline void profiler_print_timer_default_fn(timer_t* timer);
static inline void profiler_print_section_default_fn(profiler_section_t* section);
static inline void profiler_print_timers(profiler_t* profiler);
static inline void profiler_print_sections(profiler_t* profiler);
static inline void profiler_print_timer_index(profiler_t* profiler, size_t index);
static inline void profiler_print_section_index(profiler_t* profiler, size_t index);

static inline void profiler_print_timer_default_fn(timer_t* timer) {
    printf("%s - Clock Cycles %ld - Time elapsed %f\n", timer->name, timer->end - timer->start, ((timer->end - timer->start)/(float)CLOCKS_PER_SEC));
}

static inline void profiler_print_section_default_fn(profiler_section_t* section) {
    printf("Section: %s\n", section->name);
}

static inline void profiler_print_summary_default_fn(profiler_ht_timers_entry_t* entry, ui64 size) {
    for(ui64 i = 0; i < size; ++i) {
        printf("%s - Calls count: %lld - Total cycles: %ld\n", entry[i].name, entry[i].calls_count, entry[i].total_cycles);
    }
}

static inline void profiler_print_timers(profiler_t* profiler) {
    for(size_t i = 0; i < profiler->timers_count; ++i) {
        profiler->show_fn(profiler->timers + i);
    }
}

static inline void profiler_print_sections(profiler_t* profiler) {
    profiler_section_t* current_section = NULL;
    size_t i = 0;
    size_t last_index = 0;
    do {
        current_section = profiler->sections + i;
        for(size_t i = last_index; i < current_section->index; ++i) {
            profiler->show_fn(profiler->timers + i);
        }
        profiler->section_fn(current_section);
        last_index = current_section->index;
        ++i;
    } while(i < profiler->sections_count);

    for(size_t i = current_section->index; i < profiler->timers_count; ++i) {
        profiler->show_fn(profiler->timers + i);
    }
}

static inline void profiler_print_timer_index(profiler_t* profiler, size_t index) {
    profiler->show_fn(profiler->timers + index);
}

static inline void profiler_print_section_index(profiler_t* profiler, size_t index) {
    profiler_section_t* section = profiler->sections + index;
    size_t last_index = profiler->timers_count;
    if(index != profiler->sections_count - 1) {
        last_index = profiler->sections[index + 1].index;
    } 

    profiler->section_fn(section);

    for(size_t i = section->index; i < last_index; ++i) {
        profiler->show_fn(profiler->timers + i);
    }
}

void profiler_init(profiler_t* profiler) {
    profiler->timers = (timer_t*)malloc(PROFILER_DEFAUT_MEMORY * sizeof(timer_t));
    profiler->timers_table = profiler_ht_timers_init(PROFILER_DEFAUT_MEMORY);
    profiler->timers_count = 0;
    profiler->timers_capacity = PROFILER_DEFAUT_MEMORY * sizeof(timer_t);
    profiler->timer_last_stopped = profiler->timers;
    profiler->sections = (profiler_section_t*)malloc(PROFILER_DEFAUT_MEMORY * sizeof(profiler_section_t));
    profiler->sections_count = 0;
    profiler->sections_capacity = PROFILER_DEFAUT_MEMORY * sizeof(profiler_section_t);
    profiler->trace_index = 0;
    profiler->indent_string = (char*)malloc(sizeof(char) * PROFILER_INDENT_STRING_SIZE + 1);
    memset(profiler->indent_string, ' ', PROFILER_INDENT_STRING_SIZE);
}

void profiler_add_section(profiler_t* profiler, const char* name) {
    profiler->sections[profiler->sections_count++] = (profiler_section_t){
        .name = name,
        .index = profiler->timers_count
    };
}

void profiler_add_timer(profiler_t* profiler, const char* name) {
    profiler_ht_timers_set(profiler->timers_table, name);
    profiler->timers[profiler->timers_count++] = (timer_t){
        .name = name,
        .start = clock(),
        .end = 0,
        .trace_index = profiler->trace_index++
    };
}

void profiler_stop_timer(profiler_t* profiler, const char* name) {
    if(profiler->trace_index == 0) {
        printf("%s\n", "No timers running");
        return;
    }

    --profiler->trace_index;

    clock_t timer_end = clock();
    timer_t* timer = NULL;
    for(timer_t* t = profiler->timers + profiler->timers_count; t != profiler->timer_last_stopped; --t) {
        if(strcmp(name, (t - 1)->name) == 0) {
            timer = t - 1;
            break;
        }
    }

    if(!timer) {
        printf("timer %s not found\n", name);
        return;
    }

    for(timer_t* ct = timer; ct != profiler->timers + profiler->timers_count; ++ct) {
        ct->end = timer_end;
        profiler_ht_timers_update(profiler->timers_table, ct->name, timer_end - ct->start);
    }

    profiler->timer_last_stopped = timer;
}

void profiler_show_timers(profiler_t* profiler) {
    if(!profiler->show_fn) profiler->show_fn = profiler_print_timer_default_fn;
    if(!profiler->section_fn) profiler->show_fn = profiler_print_timer_default_fn;
    
    if(profiler->sections_count == 0) {
        profiler_print_timers(profiler);
        return;
    }

    profiler_print_sections(profiler);
    return;
}

void profiler_show_section(profiler_t* profiler, const char* name) {
    for(size_t i = profiler->sections_count; i > 0; --i) {
        if(strcmp(name, profiler->sections[i - 1].name) == 0) {
            profiler_print_section_index(profiler, i - 1);
            return;
        }
    }
    printf("section %s not found\n", name);
}

void profiler_show_timer(profiler_t* profiler, const char* name) {
    for(size_t i = profiler->timers_count; i > 0; --i) {
        if(strcmp(name, profiler->timers[i - 1].name) == 0) {
            profiler_print_timer_index(profiler, i - 1);
            return;
        }
    }
    printf("timer %s not found\n", name);
}

void profiler_summary(profiler_t* profiler) {
    if(!profiler->summary_fn) profiler->summary_fn = profiler_print_summary_default_fn;
    profiler_ht_timers_entry_t* entries = profiler->timers_table->entries;
    profiler->summary_fn(entries, profiler->timers_table->entries_size);
}

ui64 profiler_get_memory_usage(profiler_t* profiler) {
    return 
        profiler->timers_count * sizeof(timer_t) +
        profiler->sections_count * sizeof(profiler_section_t) +
        profiler->timers_table->indices_size * sizeof(profiler_ht_timers_entry_index_t) +
        profiler->timers_table->collisions_size * sizeof(profiler_ht_timers_entry_index_t) +
        profiler->timers_table->entries_size * sizeof(profiler_ht_timers_entry_t) +
        PROFILER_INDENT_STRING_SIZE * sizeof(char);
}

ui64 profiler_get_total_memory(profiler_t* profiler) {
    return 
        profiler->timers_capacity * sizeof(timer_t) +
        profiler->sections_capacity * sizeof(profiler_section_t) +
        profiler->timers_table->indices_capacity * sizeof(profiler_ht_timers_entry_index_t) +
        profiler->timers_table->collisions_capacity * sizeof(profiler_ht_timers_entry_index_t) +
        profiler->timers_table->entries_capacity * sizeof(profiler_ht_timers_entry_t) +
        PROFILER_INDENT_STRING_SIZE * sizeof(char);
}

void profiler_bind_show_fn(profiler_t* profiler, profiler_show_fn* show_fn) {
    profiler->show_fn = show_fn;
}

void profiler_bind_show_section_fn(profiler_t* profiler, profiler_show_section_fn* show_section_fn) {
    profiler->section_fn = show_section_fn;
}

void profiler_bind_summary_fn(profiler_t* profiler, profiler_summary_fn* summary_fn) {
    profiler->summary_fn = summary_fn;
}

void profiler_reset(profiler_t* profiler) {
    profiler->timers_count = 0;
    profiler->sections_count = 0;
    profiler->timer_last_stopped = profiler->timers;
}

void profiler_free(profiler_t* profiler) {
    profiler_ht_timers_free(profiler->timers_table);
    free(profiler->sections);
    free(profiler->timers);
    free(profiler->indent_string);
}

#undef PROFILER_DEFAUT_MEMORY
#undef PROFILER_INDENT_STRING_SIZE