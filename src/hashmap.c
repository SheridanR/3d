#include <string.h>
#include <stdint.h>
#include "hashmap.h"

hashmap_t hashmapNew(void* (*alloc)(size_t), void (*free)(void*), size_t keySize, size_t valueSize, size_t (*hash)(void*, size_t)) {
    hashmap_t hashmap;
    hashmap.buckets = vectorNew(alloc, free, sizeof(vector_t));
    for (size_t c = 0; c < hashmap.buckets.maxElements; ++c) {
        vector_t vector = vectorNew(alloc, free, keySize + valueSize);
        vectorAdd(&hashmap.buckets, &vector);
    }
    hashmap.size = 0u;
    hashmap.keySize = keySize;
    hashmap.valueSize = valueSize;
    hashmap.alloc = alloc;
    hashmap.free = free;
    hashmap.hash = hash ? hash : hashmapDjb2Hash;
    return hashmap;
}

void hashmapDelete(hashmap_t* hashmap) {
    for (size_t c = 0; c < vectorSize(&hashmap->buckets); ++c) {
        vectorDelete((vector_t*)vectorGet(&hashmap->buckets, c));
    }
    vectorDelete(&hashmap->buckets);
}

void hashmapClear(hashmap_t* hashmap) {
    for (size_t c = 0; c < vectorSize(&hashmap->buckets); ++c) {
        vector_t* bucket = (vector_t*)vectorGet(&hashmap->buckets, c);
        vectorClear(bucket);
        vectorResize(bucket, MAX_BUCKET_SIZE);
    }
    hashmap->size = 0u;
}

size_t hashmapSize(hashmap_t* hashmap) {
    return hashmap->size;
}

int hashmapEmpty(hashmap_t* hashmap) {
    return hashmap->size == 0u;
}

vector_t* hashmapGetBucket(hashmap_t* hashmap, size_t hash) {
    return (vector_t*)vectorGet(&hashmap->buckets, hash & (vectorSize(&hashmap->buckets) - 1u));
}

int hashmapAdd(hashmap_t* hashmap, void* key, void* value) {
    void* data = hashmapFind(hashmap, key);
    if (data) {
        memcpy(data, value, hashmap->valueSize);
        return 1;
    } else {
        size_t numBuckets = vectorSize(&hashmap->buckets);
        if (hashmap->size + 1u > numBuckets * MAX_BUCKET_SIZE) {
            hashmapRehash(hashmap, numBuckets * 2u);
        }

        // allocate memory to combine the pairs
        void* pair = hashmap->alloc(hashmap->keySize + hashmap->valueSize);
        memcpy(pair, key, hashmap->keySize);
        memcpy((void*)((uint8_t*)pair + hashmap->keySize), value, hashmap->valueSize);

        // produce a hash value from the key, and use it to get a bucket
        vector_t* bucket = hashmapGetBucket(hashmap, hashmap->hash(key, hashmap->keySize));
        vectorAdd(bucket, pair);

        // free the temp pair data
        hashmap->free(pair);

        ++hashmap->size;
        return 0;
    }
}

void hashmapRehash(hashmap_t* hashmap, size_t newBucketCount) {
    if (newBucketCount < 4u) {
        return;
    }

    // record existing hashmap entries to a liset
    size_t elSize = hashmap->keySize + hashmap->valueSize;
    vector_t list = vectorNew(hashmap->alloc, hashmap->free, elSize);
    iterator_t it = hashmapIteratorStart(hashmap);
    for (; !hashmapIteratorEnd(it); it = hashmapIteratorNext(it)) {
        pair_t pair = hashmapIteratorGet(it);
        vectorAdd(&list, pair.key);
    }

    // clear hashmap and resize it to the specified size
    hashmapClear(hashmap);
    for (size_t c = 0; c < hashmap->buckets.maxElements; ++c) {
        vectorDelete((vector_t*)vectorGet(&hashmap->buckets, c));
    }
    vectorClear(&hashmap->buckets);
    vectorResize(&hashmap->buckets, newBucketCount);
    for (size_t c = 0; c < hashmap->buckets.maxElements; ++c) {
        vector_t vector = vectorNew(hashmap->alloc, hashmap->free, hashmap->keySize + hashmap->valueSize);
        vectorAdd(&hashmap->buckets, &vector);
    }

    // move temp vector contents back into hashmap
    for (size_t c = 0u; c < vectorSize(&list); ++c) {
        void* key = vectorGet(&list, c);
        void* value = (void*)((uint8_t*)key + hashmap->keySize);
        hashmapAdd(hashmap, key, value);
    }

    // delete temp vector
    vectorDelete(&list);
}

int hashmapKeyCompare(hashmap_t* hashmap, void* keyA, void* keyB) {
    switch (hashmap->keySize) {
        case 1:
            return *(( uint8_t*)keyA) == *(( uint8_t*)keyB);
        case 2:
            return *((uint16_t*)keyA) == *((uint16_t*)keyB);
        case 4:
            return *((uint32_t*)keyA) == *((uint32_t*)keyB);
        case 8:
            return *((uint64_t*)keyA) == *((uint64_t*)keyB);
        default:
            for (size_t c = 0; c < hashmap->keySize; ++c) {
                if (((uint8_t*)keyA)[c] != ((uint8_t*)keyB)[c]) {
                    return 0;
                }
            }
            break;
    }
    return 1;
}

int hashmapRemove(hashmap_t* hashmap, void* key) {
    size_t hash = hashmap->hash(key, hashmap->keySize);
    vector_t* bucket = hashmapGetBucket(hashmap, hash);
    for (size_t c = 0u; c < vectorSize(bucket); ++c) {
        void* pair = vectorGet(bucket, c);
        if (hashmapKeyCompare(hashmap, pair, key)) {
            if (!vectorRemove(bucket, c)) {
                --hashmap->size;
                return 1;
            }
            break;
        }
    }
    return 0;
}

void* hashmapFind(hashmap_t* hashmap, void* key) {
    size_t hash = hashmap->hash(key, hashmap->keySize);
    vector_t* bucket = hashmapGetBucket(hashmap, hash);
    for (size_t c = 0u; c < vectorSize(bucket); ++c) {
        void* pair = vectorGet(bucket, c);
        if (hashmapKeyCompare(hashmap, pair, key)) {
            return (void*)((uint8_t*)pair + hashmap->keySize);
        }
    }
    return NULL;
}

iterator_t hashmapFindIterator(hashmap_t* hashmap, void* key) {
    iterator_t result;
    result.hashmap = hashmap;
    result.numBuckets = vectorSize(&hashmap->buckets);
    result.bucket = hashmap->hash(key, hashmap->keySize) & (vectorSize(&hashmap->buckets) - 1u);
    vector_t* bucket = vectorGet(&hashmap->buckets, result.bucket);
    for (result.position = 0u; result.position < vectorSize(bucket); ++result.position) {
        void* pair = vectorGet(bucket, result.position);
        if (hashmapKeyCompare(hashmap, pair, key)) {
            return result;
        }
    }
    result.bucket = result.numBuckets;
    result.position = 0u;
    return result;
}

iterator_t hashmapIteratorStart(hashmap_t* hashmap) {
    iterator_t result;
    result.hashmap = hashmap;
    result.position = 0u;
    result.bucket = 0u;
    result.numBuckets = vectorSize(&hashmap->buckets);
    for (; result.bucket < result.numBuckets; ++result.bucket) {
        if (vectorSize(vectorGet(&hashmap->buckets, result.bucket))) {
            break;
        }
    }
    return result;
}

int hashmapIteratorEnd(iterator_t it) {
    return it.bucket >= it.numBuckets;
}

iterator_t hashmapIteratorNext(iterator_t it) {
    ++it.position;
    while (it.bucket < it.numBuckets && it.position >= vectorSize(vectorGet(&it.hashmap->buckets, it.bucket))) {
        ++it.bucket;
        it.position = 0u;
    }
    return it;
}

pair_t hashmapIteratorGet(iterator_t it) {
    pair_t pair;
    pair.key = vectorGet(vectorGet(&it.hashmap->buckets, it.bucket), it.position);
    pair.value = (void*)((uint8_t*)pair.key + it.hashmap->keySize);
    return pair;
}

int hashmapIteratorRemove(iterator_t it) {
    int result = vectorRemoveSwap(vectorGet(&it.hashmap->buckets, it.bucket), it.position);
    if (!result) {
        --it.hashmap->size;
    }
    return result;
}

size_t hashmapDjb2Hash(void* key, size_t keySize) {
    if (key == NULL) {
        return 0u;
    }
    // djb2 hashing algorithm
    size_t value = 5381;
    for (size_t c = 0u; c < keySize; ++c) {
        size_t data = (size_t)(((uint8_t*)key)[c]);
        value = ((value << 5) + value) + data; // value = value * 33 + data
    }
    return value;
}