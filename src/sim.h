#ifndef SIM_H
#define SIM_H

#include "camera.h"
#include <stdbool.h>

extern bool running;
extern camera_t camera;

void setup();
void update();

#endif