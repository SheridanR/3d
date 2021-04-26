#ifndef DRAW_H
#define DRAW_H

#include "camera.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct window {
    uint32_t* pixels;
    float* depth;
    int width;
    int height;
} window_t;

extern window_t screen;
extern inline uint32_t color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
extern inline uint32_t get_pixel(uint32_t x, uint32_t y);
extern inline void pixel(uint32_t x, uint32_t y, uint32_t color);
extern inline bool check_depth(uint32_t x, uint32_t y, float depth);
extern inline void write_depth(uint32_t x, uint32_t y, float depth);
extern inline void check_pixel(uint32_t x, uint32_t y, uint32_t color);
extern inline void clear();
void draw();

#endif