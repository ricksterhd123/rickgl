#include <time.h>

float get_tick_count() {
    return clock() / (CLOCKS_PER_SEC / 1000);
}
