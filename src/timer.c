#include "timer.h"
#include <stdint.h>
#include <time.h>

#define FRAMES_PER_SECOND 60
#define MAX_FRAMES_BEFORE_DROP 4
static double last = 0.0;
static double diff = 0.0;
uint8_t frames_to_do = 0;
uint64_t ticks = 0;
void timer() {
    const double frame = (double)CLOCKS_PER_SEC / (double)FRAMES_PER_SECOND;
    const double now = clock();
    diff += now - last;
    if (diff >= frame) {
        diff -= frame;
        if (frames_to_do < MAX_FRAMES_BEFORE_DROP) {
            ++frames_to_do;
            ++ticks;
        }
    }
    last = clock();
}