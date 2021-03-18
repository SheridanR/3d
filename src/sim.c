#include "sim.h"
#include "camera.h"
#include "timer.h"
#include "crap.h"

bool running;
camera_t camera;

void setup() {
	running = true;

	camera.pos = vec4(0.f);
	camera.ang = (quat_t){0.f, 0.f, 0.f, 0.f};
	camera.fov = 90.f;
	camera_update(&camera);
}

void update() {
	while (frames_to_do) {
		--frames_to_do;
		if (keystatus[SDL_SCANCODE_LEFT]) {
			camera.pos.x -= 1.f;
		}
		if (keystatus[SDL_SCANCODE_RIGHT]) {
			camera.pos.x += 1.f;
		}
		if (keystatus[SDL_SCANCODE_LSHIFT]) {
			camera.pos.y += 1.f;
		}
		if (keystatus[SDL_SCANCODE_RSHIFT]) {
			camera.pos.y -= 1.f;
		}
		if (keystatus[SDL_SCANCODE_UP]) {
			camera.pos.z -= 1.f;
		}
		if (keystatus[SDL_SCANCODE_DOWN]) {
			camera.pos.z += 1.f;
		}
		camera_update(&camera);
	}
}