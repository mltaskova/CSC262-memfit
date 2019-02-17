#include "block.h"

#include <string.h>
#include <assert.h>

static size_t max(size_t a, size_t b) {
    return (a > b) ? a : b;
}

void block_init(Block_t* b, const char* name, size_t size) {
    memset(b->name, 0, NAME_LEN+1);
    b->size = size;
    for (size_t i=0; i<NAME_LEN; i++) {
        if (name[i] != '\0') {
            b->name[i] = name[i];
        } else {
            break;
        }
    }
}

Block_t* block_new(const char* name, size_t size) {
    Block_t *b = (Block_t*) malloc(sizeof(Block_t));
    block_init(b, name, size);
    return b;
}


void list_init(BlockList_t* list) {
    list->array = NULL;
    list->capacity = 0;
    list->size = 0;
}

void list_free(BlockList_t* list) {
    if (list->array != NULL) {
        free(list->array);
        list->array = NULL;
        list->size = 0;
        list->capacity = 0;
    }
}

void list_grow(BlockList_t* list) {
    assert(list != NULL);
    Block_t** data = list->array;
    size_t new_capacity = max(4,list->capacity * 2);
    list->capacity = new_capacity;
    list->array = realloc(data, sizeof(Block_t*)*new_capacity);
}

void list_push(BlockList_t* list, Block_t *block) {
    assert(list != NULL);
    if (list->size >= list->capacity) {
        list_grow(list);
    }
    assert(list->size <= list->capacity);
    list->array[list->size++] = block;
}

ssize_t list_find(BlockList_t* list, char* name) {
    for (size_t i=0; i<list->size; i++) {
        if (strcmp(list->array[i]->name, name) == 0) {
            return i;
        }
    }
    return -1;
}

Block_t* list_get(BlockList_t* list, size_t i) {
    assert(i <= list->size);
    return list->array[i];
}

Block_t* list_remove(BlockList_t* list, size_t i) {
    assert(i <= list->size);

    Block_t* retv = list->array[i];

    for (size_t j=i; j<list->size-1; j++) {
        list->array[j] = list->array[j+1];
    }
    list->array[--list->size] = NULL;
    return retv;
}

