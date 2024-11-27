#pragma once

#include <time.h>

float get_tick_count() {
    return (clock() / (float) CLOCKS_PER_SEC) * 1000;
}
