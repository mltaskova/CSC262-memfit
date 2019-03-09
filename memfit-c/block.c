#include "block.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>

int by_offset_increasing(const void* block_lhs, const void* block_rhs){
    Block* lhs = *((Block**) block_lhs);
    Block* rhs = *((Block**) block_rhs);
    assert(lhs != NULL);
    assert(rhs != NULL);

    return (int) lhs->offset - (int) rhs->offset;
}

// For qsort.
int by_size_increasing(const void* block_lhs, const void* block_rhs) {
    Block* lhs = *((Block**) block_lhs);
    Block* rhs = *((Block**) block_rhs);
    assert(lhs != NULL);
    assert(rhs != NULL);

    return (int) lhs->size - (int) rhs->size;
}

// For qsort.
int by_size_decreasing(const void* block_lhs, const void* block_rhs) {
    return by_size_increasing(block_rhs, block_lhs);
}

// for resizing.
static size_t max(size_t a, size_t b) {
    return (a > b) ? a : b;
}

static size_t min(size_t a, size_t b){
    return (a<b) ? a:b;
}

void block_init(Block* b, const char* name, size_t size) {
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

Block* block_new(const char* name, size_t size) {
    Block *b = (Block*) malloc(sizeof(Block));
    block_init(b, name, size);
    return b;
}

bool block_meets(Block* b1, Block* b2){
    if (b1->offset < b2->offset){
        return b1->offset + b1->size == b2->offset;
    }
    else{
        return b2->offset + b2->size == b1->offset;
    }
}

void list_init(BlockList* list) {
    list->array = NULL;
    list->capacity = 0;
    list->size = 0;
}

void list_free(BlockList* list) {
    if (list->array != NULL) {
        free(list->array);
        list->array = NULL;
        list->size = 0;
        list->capacity = 0;
    }
}
// making more room if list is too small
void list_grow(BlockList* list) {
    assert(list != NULL);
    Block** data = list->array;
    size_t new_capacity = max(4,list->capacity * 2);
    list->capacity = new_capacity;
    list->array = realloc(data, sizeof(Block*)*new_capacity);
}
// adding block to list
void list_push(BlockList* list, Block* block) {
    assert(list != NULL);
    if (list->size >= list->capacity) {
        list_grow(list);
    }
    assert(list->size <= list->capacity);
    list->array[list->size++] = block;
}
// find block in list by name
ssize_t list_find(BlockList* list, const char* name) {
    for (size_t i=0; i<list->size; i++) {
        if (strcmp(list->array[i]->name, name) == 0) {
            return i;
        }
    }
    return -1;
}
// get block at index i
Block* list_get(BlockList* list, size_t i) {
    assert(i <= list->size);
    return list->array[i];
}
// remove block at index
Block* list_remove(BlockList* list, size_t i) {
    assert(i <= list->size);

    Block* retv = list->array[i];

    for (size_t j=i; j<list->size-1; j++) {
        list->array[j] = list->array[j+1];
    }
    list->array[--list->size] = NULL;
    return retv;
}

void list_sort(BlockList* list, bool increasing) {
    if (increasing) {
        qsort(&list->array[0], list->size, sizeof(void*), &by_size_increasing);
    } else {
        qsort(&list->array[0], list->size, sizeof(void*), &by_size_decreasing);
    }
}

void list_shuffle(BlockList* list){
    size_t i;
    size_t j;
    Block* temp;
    for (i =0; i < list->size; i++){
        j = (size_t)((double)rand()/(RAND_MAX/list->size));
        temp = list_get(list, i);
        list->array[i] = list->array[j];
        list->array[j] = temp;
    }
}

void list_sort_by_offset(BlockList* list){
    qsort(&list->array[0], list->size, sizeof(void*), &by_offset_increasing);
}

BlockList* list_merge(BlockList* list){
    list_sort_by_offset(list);
    size_t i;
    BlockList *keep_list = (BlockList*) malloc(sizeof(BlockList));
    list_init(keep_list);
    Block* temp = block_new("", 0);
    temp->offset = 0;
    size_t temp_offset = 0;
    Block* iter;
    for (i =0; i<list->size; i++){
        iter = list_get(list, i);
        if (block_meets(temp, iter)){
            temp_offset = temp->offset;
            temp = block_new(iter->name, temp->size + iter->size);
            temp->offset = min(temp_offset, iter->offset);
        }
        else{
            if (temp->size > 0){
                list_push(keep_list, temp);
            }
            temp = iter;
        }
    }
    if (temp->size > 0){
        list_push(keep_list, temp);
    }
    return keep_list;
}

size_t list_print(BlockList* list){
    assert(list != NULL);
    size_t i;
    size_t sum = 0;
    Block* temp;
    for (i = 0; i < list->size; i++){
        temp = list_get(list, i);
        printf("%s offset %zu size %zu \n",temp->name, temp->offset, temp->size );
        sum+= temp->size;
    }
    return sum;
}












