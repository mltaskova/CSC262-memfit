#ifndef __SIMULATION_H
#define __SIMULATION_H 1

#include <stdlib.h>
#include "block.h"

typedef struct Simulation_s {
    size_t pool_size;
    BlockList_t free_list;
    BlockList_t used_list;
} Simulation_t;

void simulation_init(Simulation_t *sim);
void simulation_start(Simulation_t *sim, size_t pool);
void simulation_alloc(Simulation_t *sim, char* name, size_t amount);
void simulation_free(Simulation_t *sim, char* name);

#endif // __SIMULATION_H