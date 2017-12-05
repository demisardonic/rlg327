#ifndef MOVE_H
#define MOVE_H

#include "core.h"

int move_npc(Character*);
int move_pc();
int move_c(Character*, int, int);

int move_dijkstra(Character*, int tunneling);
int move_random(Character*, int tunneling);
int move_toward(Character*, int, int, int);

#endif

