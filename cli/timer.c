#include <stdint.h>
#include <time.h>

#ifdef _WIN32
# include <windows.h>
#endif

uint64_t monotonic_time_ms(void)
{
#ifdef _WIN32
    return (uint64_t)GetTickCount();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
#endif
}
