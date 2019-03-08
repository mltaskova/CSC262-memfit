#include "simulation.h"
#include <assert.h>
#include <stdio.h>

void simulation_init(Simulation *sim) {
    list_init(&sim->free_list);
    list_init(&sim->used_list);
    sim->pool_size = 0;
}

void simulation_start(Simulation *sim, const char* strategy, size_t pool) {
    assert(sim->free_list.size == 0);
    assert(sim->used_list.size == 0);
    sim->pool_size = pool;
    Block* temp = block_new("Pool", pool);
    temp->offset = 0;
    list_push(&sim->free_list, temp);
}

void result_print(Simulation *sim){
    list_sort_by_offset(&sim->free_list);
    list_sort_by_offset(&sim->used_list);
    printf("free: \n");
    list_print(&sim->free_list);
    printf("used: \n");
    list_print(&sim->used_list);
}

void simulation_alloc(Simulation *sim, const char* name, size_t amount) {
    assert(sim != NULL);
    list_sort(&sim->free_list, true);
    Block* temp;
    Block* split_block;
    size_t i;
    size_t new_offset = 0;
    for (i = 0; i < sim->free_list.size ; i++){
        temp = list_get(&sim->free_list, i);
        if (temp->size == amount){
            split_block = block_new(name, amount);
            split_block->offset = temp->offset;
            list_remove(&sim->free_list, i);
            list_push(&sim->used_list, split_block);
            break;
        }
        else if (temp->size > amount){
            split_block = block_new(name, amount);
            split_block->offset = temp->offset;
            list_push(&sim->used_list, split_block);
            temp->size -= amount;
            temp->offset += amount;
            break;
        }
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
}



















