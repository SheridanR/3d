#ifndef OBJ_H
#define OBJ_H

#include "vec4.h"
#include "camera.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct obj {
	bool valid;
	vec4_t* positions;
	size_t num_positions;
	uint32_t* position_indices;
	size_t num_position_indices;
	vec4_t* texture_coords;
	size_t num_texture_coords;
	uint32_t* texture_coord_indices;
	size_t num_texture_coord_indices;
	vec4_t* normals;
	size_t num_normals;
	uint32_t* normal_indices;
	size_t num_normal_indices;
} obj_t;

obj_t* load_obj(obj_t* result, const char* filename);
void free_obj(obj_t* obj);
void draw_obj(const obj_t* obj, const camera_t* camera);

#endif