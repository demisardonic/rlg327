#ifndef POINT_H
#define POINT_H

typedef struct
{
	int x, y;
} Point;

Point *point_create(int, int);

int point_delete(Point*);

int point_neighbors(Point *neighbors[8], int x, int y);

#endif

