#ifndef PROFILER_H
#define PROFILER_H

#include "timer.h"
#include "profiler_arena.h"
#include "profiler_section.h"
#include "profiler_ht_timers.h"

typedef void (profiler_show_fn)(timer_t*);
typedef void (profiler_show_section_fn)(profiler_section_t*);
typedef void (profiler_summary_fn)(profiler_ht_timers_entry_t*, ui64);

typedef struct profiler_t {
    timer_t* timers;
    profiler_ht_timers_t* timers_table;
    profiler_section_t* sections;
    size_t timers_count;
    size_t timers_capacity;
    size_t sections_count;
    size_t sections_capacity;
    ui64 trace_index;
    timer_t* timer_last_stopped;
    profiler_show_fn* show_fn;
    profiler_show_section_fn* section_fn;
    profiler_summary_fn* summary_fn;
    char* indent_string;
} profiler_t;

void profiler_init(profiler_t* profiler);
void profiler_add_timer(profiler_t* profiler, const char* name);
void profiler_add_section(profiler_t* profiler, const char* name);
void profiler_stop_timer(profiler_t* profiler, const char* name);
void profiler_show_timers(profiler_t* profiler);
void profiler_show_timer(profiler_t* profiler, const char* name);
void profiler_show_section(profiler_t* profiler, const char* name);
void profiler_summary(profiler_t* profiler);
void profiler_bind_show_fn(profiler_t* profiler, profiler_show_fn* show_fn);
void profiler_bind_show_section_fn(profiler_t* profiler, profiler_show_section_fn* show_section_fn);
void profiler_bind_summary_fn(profiler_t* profiler, profiler_summary_fn* show_section_fn);
ui64 profiler_get_memory_usage(profiler_t* profiler);
ui64 profiler_get_total_memory(profiler_t* profiler);
void profiler_reset(profiler_t* profiler);
void profiler_free(profiler_t* profiler);

#if PROFILER_ENABLED
    #define PROFILER_COLOR_WHITE "\e[0;37m"
    #define PROFILER_COLOR_RED "\x1b[31m"
    #define PROFILER_COLOR_GRAY "\x1b[30m"
    #define PROFILER_COLOR_GREEN "\x1b[32m"
    #define PROFILER_COLOR_YELLOW "\x1b[33m"
    #define PROFILER_COLOR_BLUE "\x1b[34m"
    #define PROFILER_COLOR_MAGENTA "\x1b[35m"
    #define PROFILER_COLOR_CYAN "\x1b[36m"
    #define PROFILER_COLOR_RESET "\x1b[0m"

    #define PROFILER_INDENT_STRING PROFILER_NAME.indent_string

    #define PROFILER_SETUP() profiler_t PROFILER_NAME
    #define PROFILER_INIT() profiler_init(&PROFILER_NAME)
    #define PROFILER_SECTION(NAME) profiler_add_section(&PROFILER_NAME, #NAME)
    #define PROFILER_ADD(NAME) profiler_add_timer(&PROFILER_NAME, #NAME)
    #define PROFILER_STOP(NAME) profiler_stop_timer(&PROFILER_NAME, #NAME)
    #define PROFILER_SHOW() profiler_show_timers(&PROFILER_NAME)
    #define PROFILER_SHOW_TIMER(NAME) profiler_show_timer(&PROFILER_NAME, #NAME)
    #define PROFILER_SHOW_SECTION(NAME) profiler_show_section(&PROFILER_NAME, #NAME)
    #define PROFILER_SUMMARY() profiler_summary(&PROFILER_NAME)
    #define PROFILER_MEMORY_USAGE() profiler_get_memory_usage(&PROFILER_NAME)
    #define PROFILER_TOTAL_MEMORY() profiler_get_total_memory(&PROFILER_NAME)
    #define PROFILER_BIND_SHOW_FN(FN) profiler_bind_show_fn(&PROFILER_NAME, FN)
    #define PROFILER_BIND_SHOW_SECTION_FN(FN) profiler_bind_show_section_fn(&PROFILER_NAME, FN)
    #define PROFILER_BIND_SUMMARY_FN(FN) profiler_bind_summary_fn(&PROFILER_NAME, FN)
    #define PROFILER_RESET() profiler_reset(&PROFILER_NAME)
    #define PROFILER_FREE() profiler_free(&PROFILER_NAME)

    #define PROFILER_SET_COLOR(COLOR) printf("%s", COLOR)
    #define PROFILER_RESET_COLOR(COLOR) printf("%s", PROFILER_COLOR_RESET)
#else
    #define PROFILER_INDENT_STRING

    #define PROFILER_SETUP()
    #define PROFILER_INIT()
    #define PROFILER_SECTION(NAME)
    #define PROFILER_ADD(NAME)
    #define PROFILER_STOP(NAME)
    #define PROFILER_SHOW()
    #define PROFILER_SHOW_TIMER(NAME)
    #define PROFILER_SHOW_SECTION(NAME)
    #define PROFILER_SUMMARY()
    #define PROFILER_MEMORY_USAGE()
    #define PROFILER_TOTAL_MEMORY()
    #define PROFILER_BIND_SHOW_FN(FN)
    #define PROFILER_BIND_SHOW_SECTION_FN(FN)
    #define PROFILER_BIND_SUMMARY_FN(FN)
    #define PROFILER_RESET()
    #define PROFILER_FREE()

    #define PROFILER_SET_COLOR(COLOR)
    #define PROFILER_RESET_COLOR(COLOR)
#endif

PROFILER_SETUP();

#endif