#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

inline void pixel(uint32_t x, uint32_t y, uint32_t color);
inline uint32_t color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void clear();

#endif