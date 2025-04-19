#define PROFILER_ENABLED 1
#define PROFILER_NAME profiler

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "timer.h"

#include "profiler.h"

void profiler_show_callback_old(timer_t* timer) {
    PROFILER_SET_COLOR(PROFILER_COLOR_BLUE);
    printf("%s\n", timer->name);
    PROFILER_RESET_COLOR();
    printf("    -> Cycles: %ld - Time(ms): %f\n", timer->end - timer->start, (timer->end - timer->start)/(float)CLOCKS_PER_SEC);
}

void profiler_show_callback(timer_t* timer) {
    PROFILER_SET_COLOR(PROFILER_COLOR_BLUE);
    printf("%.*s%s", timer->trace_index, PROFILER_INDENT_STRING, timer->name);
    PROFILER_RESET_COLOR();
    printf(" -> Cycles: %ld - Time(ms): %f\n", timer->end - timer->start, (timer->end - timer->start)/(float)CLOCKS_PER_SEC);
}

void profiler_show_section_custom(profiler_section_t* section) {
    PROFILER_SET_COLOR(PROFILER_COLOR_GREEN);
    #define PROFILER_SECTION_SEPARATOR "*************************************"
    printf("\n%s\n", PROFILER_SECTION_SEPARATOR);
    printf("\n%s\n", section->name);
    printf("\n%s\n\n", PROFILER_SECTION_SEPARATOR);
    PROFILER_RESET_COLOR();
    #undef PROFILER_SECTION_SEPARATOR
}

void profiler_summary_custom(profiler_ht_timers_entry_t* entry, ui64 size) {
    PROFILER_SET_COLOR(PROFILER_COLOR_YELLOW);
    #define PROFILER_SUMMARY_SEPARATOR "--------------------------------------"
    printf("\n%s\n", PROFILER_SUMMARY_SEPARATOR);
    printf("\n%s\n", "Summary");
    printf("\n%s\n\n", PROFILER_SUMMARY_SEPARATOR);
    for(ui64 i = 0; i < size; ++i) {
        printf("| %s - Calls count: %lld - Total cycles: %ld - Cycles per count avg: %lld\n", 
            entry[i].name, entry[i].calls_count, entry[i].total_cycles, entry[i].total_cycles / entry[i].calls_count);
    }
    printf("\n%s\n", PROFILER_SUMMARY_SEPARATOR);
    PROFILER_RESET_COLOR();
    #undef PROFILER_SUMMARY_SEPARATOR
}

void read_line(const char* text) {
    char input_string[50];
    printf("%s\n", text);
    fgets(input_string, 50, stdin);
}

void print_string(char* string) {
    PROFILER_ADD(PRINT_STRING);
    printf("%s\n", string);
    PROFILER_STOP(PRINT_STRING);
}

char* reverse_string(char* string) {
    PROFILER_ADD(REVERSE_STRING);
    size_t length = strlen(string);
    char* new_string = (char*)malloc(sizeof(char) * (length + 1));
    int i = 0;
    for(char* c = string + length - 1; c != string; --c) {
        new_string[i++] = *c;
    }
    new_string[i] = string[0];
    new_string[i + 1] = '\0';
    Sleep(1);
    PROFILER_STOP(REVERSE_STRING);
    return new_string;
}

int main() {

    PROFILER_INIT();

    PROFILER_BIND_SHOW_FN(profiler_show_callback);
    PROFILER_BIND_SHOW_SECTION_FN(profiler_show_section_custom);
    PROFILER_BIND_SUMMARY_FN(profiler_summary_custom);

    /* PROFILER_SECTION(EXAMPLE_SECTION);
    printf("\n\n%s\n", "EXAMPLE_SECTION");
    PROFILER_ADD(EXAMPLE);
    PROFILER_ADD(EXAMPLE1);
    read_line("Enter value 1");
    PROFILER_ADD(EXAMPLE2);
    read_line("Enter value 2");
    PROFILER_ADD(EXAMPLE3);
    read_line("Enter value 3");
    PROFILER_STOP(EXAMPLE);

    PROFILER_SHOW();
    
    PROFILER_RESET();

    PROFILER_SECTION(TEST_SECTION);
    printf("\n\n%s\n", "TEST_SECTION");
    PROFILER_ADD(TEST);
    PROFILER_ADD(TEST1);
    read_line("Enter test 1");
    PROFILER_ADD(TEST2);
    read_line("Enter test 2");
    PROFILER_ADD(TEST3);
    read_line("Enter test 3");
    PROFILER_STOP(TEST);

    PROFILER_SECTION(ANOTHER_TEST_SECTION);
    printf("\n\n%s\n", "ANOTHER_TEST_SECTION");
    PROFILER_ADD(ANOTHER_TEST);
    PROFILER_ADD(ANOTHER_TEST1);
    read_line("Enter another test 1");
    PROFILER_ADD(ANOTHER_TEST2);
    read_line("Enter another test 2");
    PROFILER_ADD(ANOTHER_TEST3);
    read_line("Enter another test 3");
    PROFILER_STOP(ANOTHER_TEST);
    
    PROFILER_SHOW(); */

    //PROFILER_SHOW_SECTION(TEST_SECTION);
    //PROFILER_SHOW_TIMER(TEST3);

    //PROFILER_RESET();

    char* string = "abcde";

    reverse_string(string);
    reverse_string(string);
    reverse_string(string);
    reverse_string(string);
    reverse_string(string);
    print_string(string);
    print_string(string);
    print_string(string);
    print_string(string);
    print_string(string);
    
    printf("Memory Usage - %lld\n", PROFILER_MEMORY_USAGE());
    printf("Total Memory - %lld\n", PROFILER_TOTAL_MEMORY());

    PROFILER_SHOW();
    PROFILER_SUMMARY();

    PROFILER_FREE();

    return 0;
}