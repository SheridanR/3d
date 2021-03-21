#include "sim.h"
#include "main.h"
#include "camera.h"
#include "timer.h"
#include "crap.h"
#include "quat.h"
#include "vec4.h"

bool running;
camera_t camera;

void setup() {
	running = true;

	camera.pos = vec4(0.f);
	camera.ang = quat();
	camera.fov = 60.f * (float)PI / 180.f;
	camera_update(&camera);
}

void update() {
	while (frames_to_do) {
		--frames_to_do;

		const float fps = 60.f;

		const float moveSpeed = (1.f / fps) * 10.f;
		const float moveX = (keystatus[SDL_SCANCODE_D] ? 1.f : 0.f) - (keystatus[SDL_SCANCODE_A] ? 1.f : 0.f);
		const float moveY = (keystatus[SDL_SCANCODE_Q] ? 1.f : 0.f) - (keystatus[SDL_SCANCODE_E] ? 1.f : 0.f);
		const float moveZ = (keystatus[SDL_SCANCODE_W] ? 1.f : 0.f) - (keystatus[SDL_SCANCODE_S] ? 1.f : 0.f);

		vec4_t move = vec4(0.f);
		const vec4_t forward = quat_to_vec3(&camera.ang);
		const vec4_t right = quat_to_vec3(mul_quat(&quat(), &camera.ang, &quat_copy(euler_to_quat(0.f, -(float)PI / 2.f, 0.f))));
		const vec4_t down = quat_to_vec3(mul_quat(&quat(), &camera.ang, &quat_copy(euler_to_quat((float)PI / 2.f, 0.f, 0.f))));
		add_vec4(&move, &vec4_copy(move), pow_vec4(&vec4(0.f), &right, moveSpeed * moveX));
		add_vec4(&move, &vec4_copy(move), pow_vec4(&vec4(0.f), &down, moveSpeed * moveY));
		add_vec4(&move, &vec4_copy(move), pow_vec4(&vec4(0.f), &forward, moveSpeed * moveZ));
		add_vec4(&camera.pos, &vec4_copy(camera.pos), &move);

		const float turnSpeed = (1.f / fps) * (float)PI / 2.f;
		const float turnX = (keystatus[SDL_SCANCODE_KP_2] ? 1.f : 0.f) - (keystatus[SDL_SCANCODE_KP_8] ? 1.f : 0.f);
		const float turnY = (keystatus[SDL_SCANCODE_KP_4] ? 1.f : 0.f) - (keystatus[SDL_SCANCODE_KP_6] ? 1.f : 0.f);
		const float turnZ = (keystatus[SDL_SCANCODE_KP_9] ? 1.f : 0.f) - (keystatus[SDL_SCANCODE_KP_7] ? 1.f : 0.f);

		mul_quat(&camera.ang, &quat_copy(camera.ang), &quat_copy(euler_to_quat(turnSpeed * turnX, 0.f, 0.f)));
		mul_quat(&camera.ang, &quat_copy(camera.ang), &quat_copy(euler_to_quat(0.f, turnSpeed * turnY, 0.f)));
		mul_quat(&camera.ang, &quat_copy(camera.ang), &quat_copy(euler_to_quat(0.f, 0.f, turnSpeed * turnZ)));

		camera_update(&camera);
	}
}