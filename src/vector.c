#include "vector.h"
#include <string.h>
#include <stdint.h>

vector_t vectorNew(void* (*alloc)(size_t), void (*free)(void*), size_t elementSize) {
    vector_t vector;
    vector.numElements = 0u;
    vector.maxElements = 4u;
    vector.elementSize = elementSize;
    vector.data = alloc(vector.maxElements * vector.elementSize);
    vector.alloc = alloc;
    vector.free = free;
    return vector;
}

void vectorDelete(vector_t* vector) {
    vector->free(vector->data);
}

void vectorClear(vector_t* vector) {
    vector->numElements = 0u;
}

size_t vectorSize(const vector_t* vector) {
    return vector->numElements;
}

int vectorEmpty(const vector_t* vector) {
    return vector->numElements == 0u;
}

void vectorAdd(vector_t* vector, const void* const data) {
    if (vector->numElements == vector->maxElements) {
        vectorResize(vector, vector->maxElements * 2u);
    }
    memcpy((void*)((uint8_t*)vector->data + vector->numElements * vector->elementSize), data, vector->elementSize);
    ++vector->numElements;
}

int vectorResize(vector_t* vector, size_t size) {
    if (size == vector->maxElements || size == 0u) {
        return 1;
    }
    void* newData = vector->alloc(size * vector->elementSize);
    if (size > vector->maxElements) {
        memcpy(newData, vector->data, vector->numElements * vector->elementSize);
    } else {
        memcpy(newData, vector->data, size * vector->elementSize);
        if (vector->numElements > size) {
            vector->numElements = size;
        }
    }
    vector->maxElements = size;
    vector->free(vector->data);
    vector->data = newData;
    return 0;
}

int vectorRemove(vector_t* vector, size_t index) {
    if (index >= vector->numElements) {
        return 1;
    }
    --vector->numElements;
    if (index < vector->numElements) {
        memmove((void*)((uint8_t*)vector->data + index * vector->elementSize),
            (const void*)((uint8_t*)vector->data + (index + 1u) * vector->elementSize),
            (vector->numElements - index) * vector->elementSize);
    }
    return 0;
}

int vectorRemoveSwap(vector_t* vector, size_t index) {
    if (index >= vector->numElements) {
        return 1;
    }
    --vector->numElements;
    if (index < vector->numElements) {
        memcpy((void*)((uint8_t*)vector->data + index * vector->elementSize),
            (const void*)((uint8_t*)vector->data + vector->numElements * vector->elementSize),
            vector->elementSize);
    }
    return 0;
}

void* vectorGet(vector_t* vector, size_t index) {
    return index < vector->numElements ? (void*)((uint8_t*)vector->data + index * vector->elementSize) : NULL;
}

int vectorCopy(vector_t* src, vector_t* dest) {
    if (src == NULL || dest == NULL) {
        return 1;
    }

    dest->free(dest->data);

    dest->alloc = src->alloc;
    dest->free = src->free;
    dest->elementSize = src->elementSize;
    dest->numElements = src->numElements;
    dest->maxElements = src->maxElements;

    dest->data = dest->alloc(dest->maxElements * dest->elementSize);
    memcpy(dest->data, src->data, src->numElements * src->elementSize);

    return 0;
}