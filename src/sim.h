#ifndef SIM_H
#define SIM_H

#include "camera.h"
#include <stdbool.h>

extern bool running;
extern camera_t camera;

void setup();
void update();

static vec4_t cube[] = {
	{0.f, 0.f, 0.f, 0.f},
	{1.f, 0.f, 0.f, 0.f},
	{1.f, 1.f, 0.f, 0.f},

	{0.f, 0.f, 0.f, 0.f},
	{1.f, 1.f, 0.f, 0.f},
	{0.f, 1.f, 0.f, 0.f},

	{0.f, 0.f, 1.f, 0.f},
	{0.f, 1.f, 1.f, 0.f},
	{1.f, 1.f, 1.f, 0.f},

	{0.f, 0.f, 1.f, 0.f},
	{1.f, 1.f, 1.f, 0.f},
	{1.f, 0.f, 1.f, 0.f},

	{0.f, 0.f, 0.f, 0.f},
	{1.f, 0.f, 1.f, 0.f},
	{1.f, 0.f, 0.f, 0.f},

	{0.f, 0.f, 0.f, 0.f},
	{0.f, 0.f, 1.f, 0.f},
	{1.f, 0.f, 1.f, 0.f},

	{0.f, 1.f, 0.f, 0.f},
	{1.f, 1.f, 0.f, 0.f},
	{1.f, 1.f, 1.f, 0.f},

	{0.f, 1.f, 0.f, 0.f},
	{1.f, 1.f, 1.f, 0.f},
	{0.f, 1.f, 1.f, 0.f},

	{0.f, 0.f, 0.f, 0.f},
	{0.f, 1.f, 0.f, 0.f},
	{0.f, 1.f, 1.f, 0.f},

	{0.f, 0.f, 0.f, 0.f},
	{0.f, 1.f, 1.f, 0.f},
	{0.f, 0.f, 1.f, 0.f},

	{1.f, 0.f, 0.f, 0.f},
	{1.f, 1.f, 1.f, 0.f},
	{1.f, 1.f, 0.f, 0.f},

	{1.f, 0.f, 0.f, 0.f},
	{1.f, 0.f, 1.f, 0.f},
	{1.f, 1.f, 1.f, 0.f},
};

#endif