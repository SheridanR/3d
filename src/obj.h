#ifndef OBJ_H
#define OBJ_H

#include "vec4.h"
#include "camera.h"
#include "mtl.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum vertex_buffer {
    VB_POSITION,
    VB_TEXTURE,
    VB_NORMAL,
    VB_NUM
} vertex_buffer_t;

typedef struct obj {
    bool valid;
    vec4_t* coords[VB_NUM];
    size_t num_coords[VB_NUM];
    uint32_t* indices[VB_NUM];
    size_t num_indices[VB_NUM];
    mtllib_t mtllib;
    mtl_t* mtl[MATERIAL_LIMIT];
    uint32_t mtl_indices[MATERIAL_LIMIT];
} obj_t;

obj_t* load_obj(obj_t* result, const char* filename);
void free_obj(obj_t* obj);
void draw_obj(const obj_t* obj, const camera_t* camera);

#endif
