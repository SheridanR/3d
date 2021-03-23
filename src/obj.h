#ifndef OBJ_H
#define OBJ_H

#include "vec4.h"
#include "camera.h"
#include <stdint.h>

typedef struct obj {
	vec4_t* positions;
	size_t num_positions;
	uint32_t* indices;
	size_t num_indices;
} obj_t;

obj_t* load_obj(obj_t* result, const char* filename);
void free_obj(obj_t* obj);
void draw_obj(const obj_t* obj, const camera_t* camera);

#endif