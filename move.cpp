#include <stdio.h>
#include <stdlib.h>

#include "character.h"
#include "debug.h"
#include "dijkstra.h"
#include "dungeon.h"
#include "move.h"
#include "point.h"
#include "util.h"

/* scan count:= number of monsters found in the scanned area */
int scc = 0;

/* scan array:= monsters found in the scanned area */
Character *scv[BUFFER_SIZE];

static int dist(int x1, int y1, int x2, int y2)
{
	int dx = ABS((x1-x2));
	int dy = ABS((y1-y2));
	return MAX(dx, dy); 
}

/* scan the area centered at (x, y) then store the result in scv */
static int scanArea(int x, int y, int radius)
{
	int r, c;
	scc = 0;

	for (r=y-radius; r<=y+radius; r++)
	{
		for (c=x-radius; c<=x+radius; c++)
		{
			int pcx, pcy;
			character_getLocation(pc, &pcx, &pcy);

			if (c==pcx && r==pcy)
				continue;
			if (r<=0 || r>=DUNGEON_H - 1)
				continue;
			if (c<=0 || c>=DUNGEON_W - 1)
				continue;
			if (cmap[r][c])
				scv[scc++] = cmap[r][c];
		}
	}
	return scc;
}

/* move c to the new location (x, y) */
static void update(Character *c, int x, int y)
{
	if (hardness[y][x])
	{
		if (hardness[y][x] > 85)
		{
			// failed to tunnel
			hardness[y][x] -= 85;
			return;
		}
		else
		{
			// tunnel successfully
			hardness[y][x] = 0;
			dungeon[y][x] = '#';
		}
	}

	int cx, cy;
	character_getLocation(c, &cx, &cy);
	
	// clear old location
	cmap[cy][cx] = NULL;
	
	debug_log("%c goes from (%d, %d) to (%d, %d)\n",
	character_getSymbol(c), cx, cy, x, y);
		
	// go to new location
	character_setLocation(c, x, y);
	
	// co:= original character occupied at new location
	Character *co = cmap[y][x];
	if (co)
	{
		if (character_isPC(co) && invulnerable)
		{
			character_setDead(c); // kill whoever attempted to kill PC
			return;
		}
		else
		{
			character_setDead(co);
		}
	}
	
	cmap[y][x] = c;
}

/* escape algorithm for PC */
static void escape()
{
	// look for optimal escape	
	int x, y, escx = 0, escy = 0, min = 1<<8;
	int pcx, pcy;
	character_getLocation(pc, &pcx, &pcy);
	
	for (x = pcx-1; x <= pcx+1; x++)
	{
		for (y = pcy-1; y <= pcy+1; y++)
		{
			if (x == pcx && y == pcy)
				continue; // skip myself
			if (hardness[y][x])
				continue; // PC cannot tunnel
			if (scanArea(x, y, 1))
				continue; // absolutely not safe

			scanArea(x, y, 2);
			if (scc < min)
			{
				min = scc;
				escx = x;
				escy = y;
			}
		}
	}
	if (escx && escy)
		update(pc, escx, escy);
	else
		move_random(pc, 0);
}

int move_npc(Character *c)
{	
	int abil = character_getAbil(c);
	int cx, cy, pcx, pcy, mempcx, mempcy;

	character_getLocation( c, & cx, & cy);
	character_getLocation(pc, &pcx, &pcy);

	character_getMemPCLocation(c, &mempcx, &mempcy);

	// intelligence
	int smart = (1 & abil);
	// telepathy
	int tele = (2 & abil);
	// tunneling
	int tunnel = (4 & abil);
	// erratic
	int erratic = (8 & abil);

	// erractic desicion or no moving instruction
	if ((erratic && rand() % 2) || (!smart && !tele)) 
	{
		move_random(c, tunnel); // 4 & c->c gives tunneling
	}
	else if (smart && tele)
	{
		move_dijkstra(c, tunnel);
	}
	else if (smart)
	{
		if (dungeon_lineOfSight(cx, cy, pcx, pcy))
		{
			move_dijkstra(c, tunnel);
			character_setMemPCLocation(c, pcx, pcy);
		}
		else if (mempcx && mempcy) // has last seen PC
		{
			move_toward(c, mempcx, mempcy, tunnel);
		}
		else
			move_random(c, tunnel);
	}
	else if (tele)
	{
		move_toward(c, pcx, pcy, tunnel); 
	}
	else
	{
		move_random(c, tunnel);
	}
	return 0;
}

int move_pc()
{
	int pcx, pcy, npcx, npcy;
	character_getLocation( pc    , & pcx, & pcy);
	character_getLocation(npcs[0], &npcx, &npcy);
	
	if (nummon <= 0)
	{
		return 0; // game over
	}
	if (invulnerable)
	{
		dijkstra(npcx, npcy, 0);
		move_dijkstra(pc, 0);
	}
	else
	{
		/* PC move algorithm */
		scanArea(pcx, pcy, 3);

		// look for monsters if no monsters are around
		if (scc == 0)
		{
			dijkstra(npcx, npcy, 0);
			move_dijkstra(pc, 0);
		}
		// fight the monster if there is only one
		else if (scc == 1)
		{
			Character *m = scv[0];
			int mx, my;
			character_getLocation(m, &mx, &my);

			int speed = character_getSpeed(m);
			int tpm = 100/speed; // turns per move
			int mvs = 0; // number of moves my enemy will take to reach me
			int mturn = character_getTurn(m);
			int pcturn = character_getTurn(pc);
			for (;pcturn - 10 <= mturn 
					&& mturn < pcturn; mturn+=tpm, mvs++);	
			int x, y, optx = 0, opty = 0, d = 1<<10;
			for (x = pcx - 1; x <= pcx + 1; x++)
			{
				for (y = pcy - 1; y <= pcy + 1; y++)
				{
					if (x == pcx && y == pcy)
						continue;
					if (hardness[y][x])
						continue;
					d = dist(x, y, mx, my);

					// if I can safely attack or my enemy cannot reach me
					if (d == 0 || d - mvs == 1)
					{
						optx = x;
						opty = y;

						if (d == 0) // attack near enemy
							break;
					}
				}
				if (d == 0) // attack near enemy
					break;
			}
			if (optx && opty)
			{
				update(pc, optx, opty);
			}
			else
			{
				escape();
			}
		}
		// escape if there are too many monsters around
		else
		{
			escape();
		}
	}

	// calculate distance map for intelligent monsters to use
	dijkstra(pcx, pcy, 0);
	dijkstra(pcx, pcy, 1);

	return 0;
}

int move_c(Character *c, int x, int y)
{
	// bad move
	if (x <= 0 || x >= DUNGEON_W - 1)
		return 1;
	if (y <= 0 || y >= DUNGEON_H - 1)
		return 1;
	if (hardness[y][x])
		return 1;

	debug_log("move %c to (%d, %d)\n",
	character_getSymbol(c), x, y);
			
	update(c, x, y);

	if (character_isPC(c))
	{
		int pcx, pcy;
		character_getLocation(pc, &pcx, &pcy);

		// calculate distance map for intelligent monsters to use
		dijkstra(pcx, pcy, 0);
		dijkstra(pcx, pcy, 1);
	}
	return 0;
}

int move_dijkstra(Character *c, int t)
{
	int x, y, cx, cy;

	character_getLocation(c, &cx, &cy);

	dijkstra_next(&x, &y, cx, cy, t);
	
	update(c, x, y);

	return 0;
}

int move_random(Character *c, int t)
{	
	int x, y;
	
	while (1)
	{
		int r = rand() % 8;

		character_getLocation(c, &x, &y);

		switch (r)
		{
			case 0: x++; break;
			case 1: x--; break;
			case 2: y++; break;
			case 3: y--; break;
			case 4: x++; y++; break;
			case 5: x--; y++; break;
			case 6: x++; y--; break;
			case 7: x--; y--; break;
		}

		// either tunneling or find a open space
		if ((t && hardness[y][x]!=IMMUTABLE) || (!hardness[y][x]))
		{
			break;
		}
		else
		{
			r = rand() % 8;	
		}
	}

	update(c, x, y);
		
	return 0;
}

int move_toward(Character *c, int x, int y, int t)
{
	int cx, cy;
	character_getLocation(c, &cx, &cy);

	int dx = x - cx;
	int dy = y - cy;

	int incx = 0;
	int incy = 0;
	if (dx > 0) incx =  1;
	if (dx < 0) incx = -1;
	if (dy > 0) incy =  1;
	if (dy < 0) incy = -1;
	
	int nextx = cx + incx;
	int nexty = cy + incy;
	
	if (t || !hardness[nexty][nextx])
		update(c, nextx, nexty);
	else
		move_random(c, t); // get stuck
	
	return 0;
}
