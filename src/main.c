#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "draw.h"
#include "crap.h"
#include "camera.h"

bool running = true;

#define FRAMES_PER_SECOND 60
#define MAX_FRAMES_BEFORE_DROP 4
static double last = 0.0;
static double diff = 0.0;
static uint8_t frames_to_do = 0;
static uint64_t ticks = 0;
static void timer() {
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

static camera_t camera;

static void setup() {
	camera.pos = vec4(0.f);
	camera.ang = (quat_t){0.f, 0.f, 0.f, 0.f};
	camera.fov = 90.f;
	camera_update(&camera);
}

static void update() {
	while (frames_to_do) {
		--frames_to_do;
		if (keystatus[SDL_SCANCODE_UP]) {
			camera.pos.z += 1.f;
		}
		if (keystatus[SDL_SCANCODE_DOWN]) {
			camera.pos.z -= 1.f;
		}
		if (keystatus[SDL_SCANCODE_LEFT]) {
			camera.pos.x += 1.f;
		}
		if (keystatus[SDL_SCANCODE_RIGHT]) {
			camera.pos.x -= 1.f;
		}
		if (keystatus[SDL_SCANCODE_LSHIFT]) {
			camera.pos.y += 1.f;
		}
		if (keystatus[SDL_SCANCODE_RSHIFT]) {
			camera.pos.y -= 1.f;
		}
		camera_update(&camera);
	}
}

int main(int argc, char* argv[]) {
	init();
	setup();
	while (running) {
		events();
		timer();
		update();
		clear();
		draw(&camera);
		refresh();
	}
	term();
	return 0;
}