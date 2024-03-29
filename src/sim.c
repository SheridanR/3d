#include "sim.h"
#include "main.h"
#include "camera.h"
#include "timer.h"
#include "crap.h"
#include "quat.h"
#include "vec4.h"
#include "obj.h"
#include <string.h>

bool running;
camera_t camera;
obj_t obj;

void setup(const char* filename) {
    running = true;

    (void)load_obj(&obj, filename);

    camera.ang = quat();
    mul_quat(&camera.ang, &quat_copy(camera.ang), &quat_copy(euler_to_quat(0.f, 0.f, (float)PI)));
    mul_quat(&camera.ang, &quat_copy(camera.ang), &quat_copy(euler_to_quat(0.f, (float)PI / 4.f, 0.f)));
    mul_quat(&camera.ang, &quat_copy(camera.ang), &quat_copy(euler_to_quat((float)PI / 6.f, 0.f, 0.f)));
    camera.pos = (vec4_t){-3.f, 3.f, 3.f, 0.f};
    camera.fov = 60.f * (float)PI / 180.f;
    camera_update(&camera);
}

static void frame(float dt) {
    const float moveSpeed = dt * 10.f;
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

    const float turnSpeed = dt * (float)PI / 2.f;
    const float turnX = (keystatus[SDL_SCANCODE_K] ? 1.f : 0.f) - (keystatus[SDL_SCANCODE_I] ? 1.f : 0.f) +
    (keystatus[SDL_SCANCODE_KP_5] ? 1.f : 0.f) - (keystatus[SDL_SCANCODE_8] ? 1.f : 0.f);
    const float turnY = (keystatus[SDL_SCANCODE_J] ? 1.f : 0.f) - (keystatus[SDL_SCANCODE_L] ? 1.f : 0.f) +
    (keystatus[SDL_SCANCODE_KP_4] ? 1.f : 0.f) - (keystatus[SDL_SCANCODE_6] ? 1.f : 0.f);
    const float turnZ = (keystatus[SDL_SCANCODE_O] ? 1.f : 0.f) - (keystatus[SDL_SCANCODE_U] ? 1.f : 0.f) +
    (keystatus[SDL_SCANCODE_KP_7] ? 1.f : 0.f) - (keystatus[SDL_SCANCODE_9] ? 1.f : 0.f);

    mul_quat(&camera.ang, &quat_copy(camera.ang), &quat_copy(euler_to_quat(turnSpeed * turnX, 0.f, 0.f)));
    mul_quat(&camera.ang, &quat_copy(camera.ang), &quat_copy(euler_to_quat(0.f, turnSpeed * turnY, 0.f)));
    mul_quat(&camera.ang, &quat_copy(camera.ang), &quat_copy(euler_to_quat(0.f, 0.f, turnSpeed * turnZ)));

    camera_update(&camera);
}

void update() {
    while (frames_to_do) {
        --frames_to_do;
        frame(1.f / FRAMES_PER_SECOND);
    }
}

void quit() {
    free_obj(&obj);
}
