#include "simulation.h"
#include <assert.h>
#include <stdio.h>

int failed_alloc = 0;
char method[25];

void simulation_init(Simulation *sim) {
    list_init(&sim->free_list);
    list_init(&sim->used_list);
    sim->pool_size = 0;
}

void simulation_start(Simulation *sim, const char* strategy, size_t pool) {
    assert(sim->free_list.size == 0);
    assert(sim->used_list.size == 0);
    sim->pool_size = pool;
    strncpy(method, strategy, 25);
    printf("%s\n", method);
    Block* temp = block_new("Pool", pool);
    temp->offset = 0;
    list_push(&sim->free_list, temp);
}

void result_print(Simulation *sim){
    size_t sum;
    list_sort_by_offset(&sim->free_list);
    list_sort_by_offset(&sim->used_list);
    sum = list_print(&sim->free_list);
    printf("free: %lf\n", (double)sum/sim->pool_size);
    sum = list_print(&sim->used_list);
    printf("used: %lf\n", (double)sum/sim->pool_size);
    printf("failed alloc %d times\n", failed_alloc);
}

void simulation_alloc(Simulation *sim, const char* name, size_t amount) {
    assert(sim != NULL);
    if (strncmp(method, "best", 4) == 0){
        list_sort(&sim->free_list, true); 
    }
    else if (strncmp(method, "worst", 4) == 0) {
        list_sort(&sim->free_list, false);
    }
    else if (strncmp(method, "first", 4) == 0) {
        list_sort_by_offset(&sim->free_list);
    }
    else if (strncmp(method, "random", 4) == 0){
        list_shuffle(&sim->free_list);
    }
    Block* temp;
    Block* split_block;
    bool success = false;
    size_t i;
    size_t new_offset = 0;
    for (i = 0; i < sim->free_list.size ; i++){
        temp = list_get(&sim->free_list, i);
        if (temp->size == amount){
            split_block = block_new(name, amount);
            split_block->offset = temp->offset;
            list_remove(&sim->free_list, i);
            list_push(&sim->used_list, split_block);
            success = true;
            break;
        }
        else if (temp->size > amount){
            split_block = block_new(name, amount);
            split_block->offset = temp->offset;
            list_push(&sim->used_list, split_block);
            temp->size -= amount;
            temp->offset += amount;
            success = true;
            break;
        }
    }
    if (!success){
        // printf("could not alloc %s\n", name);
        failed_alloc++;
    }
}

void simulation_free(Simulation *sim, const char* name) {
    ssize_t position = list_find(&sim->used_list, name);
    if (position < 0) {
        fprintf(stderr, "Could not find previously-allocated: %s\n", name);
        exit(-2);
    }
    Block* found = list_remove(&sim->used_list, (size_t) position);
    assert(found != NULL);
    list_push(&sim->free_list, found);
    sim->free_list = *list_merge(&sim->free_list);
}



















