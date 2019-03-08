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
    list_sort(&sim->free_list, true);
    list_sort(&sim->used_list, true);
    list_print(&sim->free_list);
    list_print(&sim->used_list);
}

void simulation_alloc(Simulation *sim, const char* name, size_t amount) {
    assert(sim != NULL);
    // for now, conjure blocks from thin-air;
    // TODO: do something more reasonable.
    list_sort(&sim->free_list, false);
    Block* temp;
    size_t i;
    size_t new_offset = 0;
    for (i = 0; i < sim->free_list.size; i++){
        temp = list_get(&sim->free_list, i);
        if (temp->size == amount){
            list_push(&sim->used_list, list_remove(&sim->free_list, i));
            break;
        }
        else if (temp->size > amount){
            list_push(&sim->used_list, block_new(name, amount));
            temp->size -= amount;
            new_offset = temp->offset;
            temp->offset += amount;
            break;
        }
        else{
            fprintf(stderr, "alloc %s failed \n", name);
            break;
        }
    }
    i = list_find(&sim->used_list, name);
    temp = list_get(&sim->used_list, i);
    temp->offset = new_offset;
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



















