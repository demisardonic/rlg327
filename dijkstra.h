#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "core.h"

extern int visited[DUNGEON_H][DUNGEON_W];

extern int distN[DUNGEON_H][DUNGEON_W]; // non-tunneling
extern int distT[DUNGEON_H][DUNGEON_W]; // tunneling

int dijkstra_next(int*, int*, int, int, int tunneling);
int dijkstra_print(int tunneling);

/* dijkstra starting at (x, y) */
int dijkstra(int x, int y, int tunneling);

#endif

