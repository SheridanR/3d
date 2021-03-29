#include "mtl.h"
#ifdef TEXTURES
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static texture_t* load_texture(texture_t* result, const char* filename) {
#ifndef TEXTURES
    return NULL;
#else
    SDL_Surface* surf;
    if ((surf = IMG_Load(filename)) == NULL) {
        return NULL;
    } else {
        strcpy(result->filename, filename);
        result->surface = surf;
        result->pixels = surf->pixels;
        result->width = surf->w;
        result->height = surf->h;
        return result;
    }
#endif
}

static void free_texture(texture_t* texture) {
#ifdef TEXTURES
    if (texture->surface) {
        SDL_FreeSurface((SDL_Surface*)texture->surface);
    }
#endif
}

mtllib_t* load_mtllib(mtllib_t* result, const char* filename) {
    char buf[1024];
    memset(result, 0, sizeof(mtllib_t));
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        return NULL;
    }
    mtl_t* mtl = NULL;
    size_t new_mtl_index = 0;
    while (fgets(buf, sizeof(buf), fp)) {
        if (buf[0] != '#' && buf[0] != '\n') {
            char* type = strtok(buf, " ");
            if (strcmp(type, "newmtl") == 0) {
                mtl = &result->materials[new_mtl_index++];
                char buf[128] = {'\0'};
                char* name = type + strlen(type) + 1;
                char* newline = strrchr(name, '\r');
                if (!newline) {
                    newline = strrchr(name, '\n');
                }
                strncpy(buf, name, newline - name);
                strcpy(mtl->filename, buf);
            }
            else if (strcmp(type, "Kd") == 0) {
                char* c1 = strtok(NULL, " "); float f1 = strtof(c1, NULL);
                char* c2 = strtok(NULL, " "); float f2 = strtof(c2, NULL);
                char* c3 = strtok(NULL, " "); float f3 = strtof(c3, NULL);
                mtl->diffuse = (vec4_t){f1, f2, f3, 1.f};
            }
            else if (strcmp(type, "map_Kd") == 0) {
                char buf[128] = {'\0'};
                char* prefix = strrchr(filename, '/');
                if (!prefix) {
                    prefix = strrchr(filename, '\\');
                }
                strncpy(buf, prefix, (size_t)(prefix - filename + 1));
                char* name = strtok(NULL, " \r\n");
                strcat(buf, name);
                (void)load_texture(&mtl->diffuse_texture, buf);
            }
        }
    }
    fclose(fp);
    return result;
}

void free_mtllib(mtllib_t* mtllib) {
    for (int c = 0; c < MATERIAL_LIMIT; ++c) {
        free_texture(&mtllib->materials[c].diffuse_texture);
    }
}