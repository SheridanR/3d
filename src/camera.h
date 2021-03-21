#ifndef CAMERA_H
#define CAMERA_H

#include "vec4.h"
#include "matrix.h"
#include "quat.h"

#define CAMERA_CLIP_NEAR 1

typedef struct camera {
	float fov;		// field-of-view (radians)
	vec4_t pos;		// position
	quat_t ang;		// angle
	mat4x4_t proj;	// auto-populated
	mat4x4_t view;	// auto-populated
} camera_t;

vec4_t world_to_screen_coords(const vec4_t* world_coords, const camera_t* camera);
void camera_update(camera_t* camera);

#endif