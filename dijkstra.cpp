#include <stdio.h>
#include <stdlib.h>

#include "point.h"
#include "dijkstra.h"
#include "util.h"
#include "heap.h"

Point *arr[DUNGEON_H * DUNGEON_W];

int arrsize = 0;
int visited[DUNGEON_H][DUNGEON_W];
int distN[DUNGEON_H][DUNGEON_W];
int distT[DUNGEON_H][DUNGEON_W];
// compare two points in non-tunneling distance view
static int comparePointN(const void *pv, const void *qv)
{
	Point *p = (Point *) pv;
	Point *q = (Point *) qv;
	return distN[p->y][p->x] - distN[q->y][q->x];
}

// compare two points in tunneling distance view
static int comparePointT(const void *pv, const void *qv)
{
	Point *p = (Point *) pv;
	Point *q = (Point *) qv;
	return distT[p->y][p->x] - distT[q->y][q->x];
}

int dijkstra_next(int *nextx, int *nexty, int x, int y, int t)
{
	int closest = 1<<8;

	Point *ns[8];
	point_neighbors(ns, x, y);
	
	if (t)
	{
		int i;
		for(i=0; i<8; i++)
		{
			Point *n = ns[i];
			if (distT[n->y][n->x] < closest)
			{
				x = n->x;
				y = n->y;
				closest = distT[n->y][n->x];
			}
			point_delete(n);
		}
	}
	else
	{
		int i;
		for(i=0; i<8; i++)
		{
			Point *n = ns[i];
			if (distN[n->y][n->x] < closest)
			{
				x = n->x;
				y = n->y;
				closest = distN[n->y][n->x];
			}
			point_delete(n);
		}
	}

	*nextx = x;
	*nexty = y;
	
	return 0;
}

int dijkstra_print(int t)
{
	int r, c;

	for (r=0; r<DUNGEON_H; r++)
	{
		for (c=0; c<DUNGEON_W; c++)
		{
			char ch = util_base62((t ? distT[r][c] : distN[r][c]));
			
			if (ch<0)
				printf("%c", dungeon[r][c]);
			else
				printf("%c", ch);
		}
		printf("\n");
	}
	return 0;
}

int dijkstra(int srcx, int srcy, int t)
{
	Point *src = point_create(srcx, srcy);

	// init
	int r, c;
	for (r=0; r<DUNGEON_H ; r++)
	{
		for (c=0; c<DUNGEON_W; c++)
		{
			if(t)
				distT[r][c] = 1<<10;
			else
				distN[r][c] = 1<<10;
			
			visited[r][c] = 0;
		}
	}
	
	// set src to have distance 0
	if (t)
		distT[src->y][src->x] = 0;
	else
		distN[src->y][src->x] = 0;

	Heap *h = heap_new(t ? comparePointT : comparePointN);
	
	heap_insert(h, src);

	Point *min;
	
	while ((min = (Point *) heap_extract(h)))
	{
		int x = min->x;
		int y = min->y;
		point_delete(min);
		
		visited[y][x] = 1;

		Point *ns[8]; // neighbors
		
		point_neighbors(ns, x, y);
		
		int weight = util_weight(hardness[y][x]);
		
		int alt = weight + (t ? distT[y][x] : distN[y][x]);

		int i;
		for(i=0; i<8; i++)
		{
			Point *p = ns[i];
			int px = p->x;
			int py = p->y;
			point_delete(p);

			if (visited[py][px] ||
					py < 1 || py >=DUNGEON_H-1 || 
					px < 1 || px >=DUNGEON_W-1)
			{
				continue;
			}
			int hard = hardness[py][px];

			if (!t && hard>0)
			{
				continue;
			}

			int weight = util_weight(hard);
				
			if (weight<0)
			{
				continue;
			}
			
			if (alt < (t ? distT[py][px] : distN[py][px]))
			{
				heap_insert(h, point_create(px, py));
				
				if (t)
					distT[py][px] = alt;
				else
					distN[py][px] = alt;
			}
		}
	}

	heap_delete(h);

	return 0;
}
