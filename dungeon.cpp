#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "core.h"
#include "character.h"
#include "debug.h"
#include "dungeon.h"
#include "path.h"
#include "pc.h"
#include "room.h"
#include "util.h"

static void print(int r, int c, char ch)
{
  mvprintw(r+1, c, "%c", ch);
}

// used only when debug
int sightmap[DUNGEON_H][DUNGEON_W];

int dungeon_delete()
{
  int r, c;
  
  while(roomc>0)
    {
      room_delete(rooms[--roomc]);
    }
  
  for (r = 0; r < DUNGEON_H; r++)
    {
      for (c = 0; c < DUNGEON_W; c++)
	{
	  cmap[r][c] = NULL;
	}
    }
  
  return 0;
}

int dungeon_filesize()
{
  return strlen(MARKER) + sizeof(int) * 2 + MUTABLE_SIZE + roomc * 4;
}

int dungeon_fill(char ch)
{
  int r, c;
  for(r=1;r<DUNGEON_H-1;r++)
    {
      for(c=1;c<DUNGEON_W-1;c++)
	{
	  dungeon[r][c] = ch;
	  
	  if(ch==ROCK)
	    {
	      hardness[r][c] = util_rnum(1, 255 - 1);
	    }
	}
    }
  return 0;
}

int dungeon_print()
{
	// build sight map
	if (sight)
	{
		// clear old sight map
		int r, c;
		for(r=0;r<DUNGEON_H;r++)
		{
			for(c=0;c<DUNGEON_W;c++)
			{
				sightmap[r][c] = 0;
			}
		}
		int i;
		for(i = 0; i<nummon; i++)
		{
			int npcx, npcy, pcx, pcy;
			character_getLocation(npcs[i], &npcx, &npcy);
			character_getLocation( pc    , & pcx, & pcy);
			dungeon_lineOfSight(npcx, npcy, pcx, pcy);
		}
	}

	int r, c;
	for(r=0;r<DUNGEON_H;r++)
	{
		for(c=0;c<DUNGEON_W;c++)
		{
			int visible = dungeon_isVisible(c, r);

			if ((nofog || visible) && cmap[r][c])
			{
				int speed = character_getSpeed(cmap[r][c]);

				int color = 0;
				if (character_isPC(cmap[r][c])) // if it is PC
				{
					init_pair(color = 1, COLOR_RED, COLOR_YELLOW);
				}
				else if (100/speed >= 15)
					init_pair(color = 2, COLOR_WHITE, COLOR_BLACK);
				else if (100/speed >= 10)
					init_pair(color = 3, COLOR_GREEN, COLOR_BLACK);
				else if (100/speed >= 7)
					init_pair(color = 4, COLOR_CYAN, COLOR_BLACK);
				else if (100/speed >= 5)
					init_pair(color = 5, COLOR_MAGENTA, COLOR_BLACK);
				
				attron(COLOR_PAIR(color));
				
				// print character on the dungeon
				print(r, c, character_getSymbol(cmap[r][c]));
				attroff(COLOR_PAIR(color));
			}
			else if (sight && sightmap[r][c]) // only for debug
			{
				int color;
				if (sightmap[r][c] > 0)
				{	
					init_pair(color = 3, COLOR_GREEN, COLOR_BLACK);
					attron(COLOR_PAIR(color));
					print(r, c, 'o');
				}
				else
				{
					init_pair(color = 30, COLOR_RED, COLOR_BLACK);
					attron(COLOR_PAIR(color));
					print(r, c, 'x');
				}
				attroff(COLOR_PAIR(color));
			}
			else
			{
				// print dungeon cell on the dungeon
				char *seenDungeon = pc_getSeenDungeon();
				
				char ch = seenDungeon[r*DUNGEON_W + c];
				
				int color = 10;
				if (visible)
				{
					init_pair(color, COLOR_YELLOW, COLOR_BLACK);
					attron(COLOR_PAIR(color));
				}
				print(r, c, ch);
				attroff(COLOR_PAIR(color));

				if (nofog)
					print(r, c, dungeon[r][c]);
			}
		}
	}
	return 0;
}

int dungeon_generate()
{
	debug_log("generating new dungeon...\n");

	dungeon_fill(ROCK); // fill the dungeon with rocks

	unsigned int i, j, t;
	for(i=0, t=0; t<tol; t++)
	{
		rooms[i] = room_new();

		// determine if the new room collides with any previous room	
		int hasCollision = 0;
		for(j=0;j<i;j++)
		{
			if(room_collide(rooms[i], rooms[j]))
			{
				hasCollision = 1;
				break;
			}
		}
	
		if(hasCollision)
		{
			room_delete(rooms[i]);
		}
		else		
		{
			room_paint(rooms[i]);

			// sparse path
			path_pave(rooms[i], rooms[0]);

			// dense path
			if(dense)
			{
				for(j=1;j<i;j++)
				{
					path_pave(rooms[i], rooms[j]);
				}
			}

			i++;
		}
	}
	roomc = i;

	// place stairs
	int x, y;
	room_randomLocation(&x, &y, rooms[0]);
	dungeon[y][x] = STAIR_UP;
	room_randomLocation(&x, &y, rooms[1]);
	dungeon[y][x] = STAIR_DOWN;

	return 0;
}

int dungeon_isVisible(int x, int y)
{
	int pcx, pcy, r = 3;
	character_getLocation(pc, &pcx, &pcy);

	if (!dungeon_lineOfSight(pcx, pcy, x, y)) return 0;

	return (pcx-r<=x && pcx+r>=x && pcy-r<=y && pcy+r>=y) 
		|| room_isInSameRoom(x, y, pcx, pcy);
}

int dungeon_lineOfSight(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	int incx = 0;
	int incy = 0;

	if (dx > 0) incx =  1;
	if (dx < 0) incx = -1;
	if (dy > 0) incy =  1;
	if (dy < 0) incy = -1;

	int curx = x1;
	int cury = y1;

	int blocked = 0;

	while (curx!=x2 || cury!=y2)
	{

		if (hardness[cury][curx])
			blocked = 1;
		
		if (cmap[y1][x1] && cmap[y2][x2])
		{
			if (blocked)
				sightmap[cury][curx] = -1; // -1 means blocked
			else
				sightmap[cury][curx] =  1; //  1 means not blocked
		}

		if (ABS(dx)>ABS(dy))
		{
			curx += incx;
			cury = y1 + (curx - x1) * dy / dx;
		}
		else
		{
			cury += incy;
			curx = x1 + (cury - y1) * dx / dy;
		}
	}
	return !blocked;
}

int dungeon_load()
{
	FILE *ifp = fopen(loadp, "r");
	if(!ifp)
	{
		fprintf(stderr, "Could not load file: %s not exist\n", loadp);
		return 1;
	}

	char buffer[BUFFER_SIZE];

	// 0-5 RLG327
	fread(buffer, sizeof(*buffer), 6, ifp);
	buffer[6] = '\0';	

	// 6-13
	unsigned int buf32[2];
	fread(buf32, sizeof(*buf32), 2, ifp);

	debug_log("debug: load 0-5: %s\n", buffer);
	debug_log("debug: load 6-9: %u\n", util_reverse(buf32[0])); 
	debug_log("debug: load 10-13: %u\n", util_reverse(buf32[1]));
	
	if(strcmp(buffer, "RLG327"))
	{
		fprintf(stderr, "Could not load file: %s: not a RLG327\n", loadp);
		fclose(ifp);
		return 1;
	}

	// 14-1495
	unsigned char buf8[MUTABLE_SIZE];
	fread(buf8, sizeof(*buf8), MUTABLE_SIZE, ifp);
	int r, c;
	for(r=1;r<DUNGEON_H-1;r++)
	{
		for(c=1;c<DUNGEON_W-1;c++)
		{
			int index = (r-1)*78+(c-1);
		
			hardness[r][c] = buf8[index];

			// floor
			if(!buf8[index])
				dungeon[r][c] = PATH;
			else
				dungeon[r][c] = ROCK;
		}
	}

	int i=0;
	
	// 1495-end
	while(fread(buf8, sizeof(*buf8), 4, ifp))
	{
		debug_log("debug: load room %d: x=%u, y=%u, w=%u, h=%u\n",
		i, buf8[0], buf8[1], buf8[2], buf8[3]);

		rooms[i] = (Room *)malloc(sizeof(*rooms));

		rooms[i]->x = buf8[0];
		rooms[i]->y = buf8[1];
		rooms[i]->w = buf8[2];
		rooms[i]->h = buf8[3];

		room_paint(rooms[i]);

		i++;
	}
	roomc = i;

	debug_log("debug: load room count: %d\n", roomc);
	
	fclose(ifp);

	return 0;
}

int dungeon_save()
{
	FILE *ofp = fopen(savep, "w");
	if(!ofp)
	{
		fprintf(stderr, "Could not save file: %s\n", savep);
		return 1;
	}
	
	// 0-5 RLG327
	fwrite("RLG327", 6, 1, ofp);
	
	// 6-13
	unsigned int buf32[2];
	buf32[0] = util_reverse(0);
	buf32[1] = util_reverse(dungeon_filesize());
	fwrite(buf32, sizeof(*buf32), 2, ofp);

	unsigned char buf8[MUTABLE_SIZE];
	
	// 14-1495
	int r, c;
	for(r=1;r<DUNGEON_H-1;r++)
	{
		for(c=1;c<DUNGEON_W-1;c++)
		{
			int index = (r-1)*78+(c-1);
			
			buf8[index] = hardness[r][c];
		}
	}
	fwrite(buf8, sizeof(*buf8), MUTABLE_SIZE, ofp);

	// 1495-end
	int i;
	for(i=0;i<roomc;i++)
	{
		buf8[0] = rooms[i]->x;
		buf8[1] = rooms[i]->y;
		buf8[2] = rooms[i]->w;
		buf8[3] = rooms[i]->h;	
		
		fwrite(buf8, sizeof(*buf8), 4, ofp);
	}

	fclose(ofp);
		
	return 0;
}
