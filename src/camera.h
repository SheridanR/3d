#ifndef CAMERA_H
#define CAMERA_H

#include "vec4.h"
#include "matrix.h"
#include "quat.h"
#include <stdbool.h>

#define CAMERA_CLIP_NEAR 1

typedef struct frustum {
    vec4_t planes[5];
} frustum_t;

typedef struct camera {
    float fov;          // field-of-view (radians)
    vec4_t pos;         // position
    quat_t ang;         // angle
    mat4x4_t proj;      // auto-populated
    mat4x4_t view;      // auto-populated
    frustum_t frustum;  // view frustum
} camera_t;

vec4_t project_to_screen(const vec4_t* coords, const camera_t* camera);
void camera_update(camera_t* camera);

#endif
