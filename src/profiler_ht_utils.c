#include "profiler_ht_utils.h"

size_t profiler_ht_hash(const char* string) {
    ui32 hash = 2166136261u;
    size_t length = strlen(string);
    for(const char* temp = string; temp != string + length; ++temp) {
        hash ^= (ui8)*temp;
        hash *= 16777619;
    }
    return hash;
}