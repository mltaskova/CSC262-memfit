#include "simulation.h"
#include <assert.h>

void simulation_init(Simulation *sim) {
    list_init(&sim->free_list);
    list_init(&sim->used_list);
    sim->pool_size = 0;
}

void simulation_start(Simulation *sim, size_t pool) {
    assert(sim->free_list.size == 0);
    assert(sim->used_list.size == 0);
    sim->pool_size = pool;
    list_push(&sim->free_list, block_new("Pool", pool));
}

void simulation_alloc(Simulation *sim, char* name, size_t amount) {

}

void simulation_free(Simulation *sim, char* name) {

}
