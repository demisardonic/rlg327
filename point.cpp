#include <stdlib.h>

#include "point.h"

Point *point_create(int x, int y){
  Point *p = (Point *) malloc(sizeof(*p));
  p->x = x;
  p->y = y;
  return p;
}

int point_delete(Point *p)
{
	free(p);
	return 0;
}

int point_neighbors(Point *ns[8], int x, int y)
{	
	ns[0] = point_create(x, y - 1); // N
	ns[1] = point_create(x, y + 1); // S
	ns[2] = point_create(x - 1, y); // W
	ns[3] = point_create(x + 1, y); // E
	ns[4] = point_create(x - 1, y - 1); // NW
	ns[5] = point_create(x + 1, y - 1); // NE
	ns[6] = point_create(x - 1, y + 1); // SW
	ns[7] = point_create(x + 1, y + 1); // SE
	return 0;
}

