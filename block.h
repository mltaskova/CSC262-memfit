#ifndef __BLOCK_H
#define __BLOCK_H 1

#include <stdlib.h>
#include <string.h>

#define NAME_LEN 24

typedef struct {
  size_t size;
  char name[NAME_LEN+1];
} Block_t;

typedef struct {
  size_t size;
  size_t capacity;
  Block_t** array;
} BlockList_t;

void block_init(Block_t* b, const char* name, size_t size);
Block_t* block_new(const char* name, size_t size);

void list_init(BlockList_t* list);
void list_free(BlockList_t* list);
void list_push(BlockList_t* list, Block_t *block);
ssize_t list_find(BlockList_t* list, char* name);
Block_t* list_get(BlockList_t* list, size_t i);
Block_t* list_remove(BlockList_t* list, size_t i);

#endif // __BLOCK_H