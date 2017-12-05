#include <stdlib.h>

#include "ncurses.h"
#include "core.h"
#include "debug.h"
#include "pc.h"

#include "dungeon.h"

__PC::__PC() : __Character()
{
	pc = true;

	speed = 10;
	symbol = '@';

	seenDungeon = (char *)malloc(DUNGEON_W * DUNGEON_H);

	clearSeenDungeon();
}

__PC::~__PC()
{
	free(seenDungeon);
}

void __PC::clearSeenDungeon()
{
	for (int i = 0; i < DUNGEON_W * DUNGEON_H; i++)
		seenDungeon[i] = ' ';
}

void __PC::setLocation(int x, int y)
{
	__Character::setLocation(x, y);
		
	for (int r=0; r<DUNGEON_H; r++)
	{
		for (int c=0; c<DUNGEON_W; c++)
		{
			if (dungeon_isVisible(c, r))
				seenDungeon[r*DUNGEON_W+c]=dungeon[r][c];
		}
	}
}

/* singleton pc */
Character *pc;


int pc_clearSeenDungeon()
{
	((__PC *)pc)->clearSeenDungeon();
	return 0;
}

char *pc_getSeenDungeon()
{
	return ((__PC *)pc)->seenDungeon;
}


