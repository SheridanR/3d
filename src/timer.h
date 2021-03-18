#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

extern uint8_t frames_to_do;
extern uint64_t ticks;

void timer();

#endif