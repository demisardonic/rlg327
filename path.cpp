#include <stdio.h>
#include <time.h>

#include "core.h"
#include "room.h"
#include "util.h"

static int numAdjacentPaths(int r, int c)
{
	int num = 0;
	if(dungeon[r-1][c]==PATH)
		num++;
	if(dungeon[r+1][c]==PATH)
		num++;
	if(dungeon[r][c-1]==PATH)
		num++;
	if(dungeon[r][c+1]==PATH)
		num++;
	return num;
}

void path_pave(Room* from, Room* to)
{
	int fromx, fromy, tox, toy;
	
	room_centroid(&fromx, &fromy, from);
	room_centroid(&tox, &toy, to);

	int dx = tox - fromx;
	int dy = toy - fromy;

	int incx = dx > 0 ? 1 : -1;
	int incy = dy > 0 ? 1 : -1;
	
	int r = fromy, c = fromx;

	while(r!=toy && c!=tox)
	{	
		if(dungeon[r][c] != ROOM)
		{
			dungeon[r][c] = PATH;
			hardness[r][c] = 0;
		}

		// if we connect to existing path, then stop paving
		if(numAdjacentPaths(r, c)>1)
			break;
		
		// prefer less hard path
		if(hardness[r][c+incx]<hardness[r+incy][c])
			c+=incx;
		else
			r+=incy;
	}
	while(r != toy || c != tox)
	{
		if(dungeon[r][c] != ROOM)
		{
			dungeon[r][c] = PATH;
			hardness[r][c] = 0;
		}

		// if we connect to existing path, then stop paving
		if(numAdjacentPaths(r, c)>1)
			break;

		if(r!=toy)
			r+=incy;
		if(c!=tox)
			c+=incx;
	}
}
