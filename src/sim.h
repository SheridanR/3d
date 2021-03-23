#ifndef SIM_H
#define SIM_H

#include "camera.h"
#include "obj.h"
#include <stdbool.h>

extern bool running;
extern camera_t camera;
extern obj_t obj;

void setup(const char* filename);
void update();
void close();

#endif