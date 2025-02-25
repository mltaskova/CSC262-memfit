#ifndef __BLOCK_H
#define __BLOCK_H 1

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NAME_LEN 24

typedef struct {
  size_t offset;
  size_t size;
  char name[NAME_LEN+1];
} Block;

typedef struct {
  size_t size;
  size_t capacity;
  Block** array;
} BlockList;

// Block methods
void block_init(Block* b, const char* name, size_t size);
Block* block_new(const char* name, size_t size);
bool block_meets(Block* b1, Block* b2);

// BlockList methods.
void list_init(BlockList* list);
void list_free(BlockList* list);
void list_push(BlockList* list, Block *block);
ssize_t list_find(BlockList* list, const char* name);
Block* list_get(BlockList* list, size_t i);
Block* list_remove(BlockList* list, size_t i);
void list_sort(BlockList* list, bool increasing);
size_t list_print(BlockList* list);
void list_sort_by_offset(BlockList* list);
BlockList* list_merge(BlockList* list);
void list_shuffle(BlockList* list);


#endif // __BLOCK_H