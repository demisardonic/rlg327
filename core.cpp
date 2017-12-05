#include <stdio.h>

#include "core.h"

char dungeon[DUNGEON_H][DUNGEON_W];

unsigned char hardness[DUNGEON_H][DUNGEON_W];
	
int core_init()
{
	// init immutable borders
	int r, c;
	for(r=0;r<DUNGEON_H;r++)
	{
		dungeon[r][0] = ROCK;
		dungeon[r][DUNGEON_W-1] = ROCK;
			
		hardness[r][0] = IMMUTABLE;
		hardness[r][DUNGEON_W-1] = IMMUTABLE;
	}
	for(c=0;c<DUNGEON_W;c++)
	{
		dungeon[0][c] = ROCK;
		dungeon[DUNGEON_H-1][c] = ROCK;
		
		hardness[0][c] = IMMUTABLE;
		hardness[DUNGEON_H-1][c] = IMMUTABLE;
	}
	return 0;
}

