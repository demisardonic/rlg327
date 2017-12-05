#include <stdlib.h>
#include <time.h>

#include "core.h"
#include "util.h"
#include "room.h"

Room *rooms[BUFFER_SIZE];

int roomc;

void room_centroid(int* x, int* y, Room* r){
  *x = r->x + r->w/2;
  *y = r->y + r->h/2;
}

int room_collide(Room* r, Room* s){
  if(r->x+r->w<s->x || r->x>s->x+s->w)
    return 0;
  if(r->y+r->h<s->y || r->y>s->y+s->h)
    return 0;
  
  return 1;
}

int room_contains(Room *r, int x, int y){
  return r->x <= x && r->x + r->w > x &&
    r->y <= y && r->y + r->h > y;
}

Room *room_new(){
  Room *r = (Room *) malloc(sizeof(*r));
  r->w = util_rnum(ROOM_MIN_W, ROOM_MAX_W);
  r->h = util_rnum(ROOM_MIN_H, ROOM_MAX_H);
  r->x = util_rnum(1, DUNGEON_W - r->w - 1);
  r->y = util_rnum(1, DUNGEON_H - r->h - 1);
  return r;
}

int room_delete(Room *room)
{
	if(!room)
		return 1;
	
	free(room);
	return 0;
}

int room_isInSameRoom(int x1, int y1, int x2, int y2)
{
	int i;
	for(i=0; i<roomc; i++)
	{
		if (room_contains(rooms[i], x1, y1) &&
			room_contains(rooms[i], x2, y2))
			return 1;
	}
	return 0;
}

void room_paint(Room *room)
{
	int r, c;
	for(r=room->y;r<room->y+room->h;r++)
	{
		for(c=room->x;c<room->x+room->w;c++)
		{
			dungeon[r][c] = ROOM;
			hardness[r][c] = 0;
		}
	}
}

void room_randomLocation(int *x, int *y, Room *r)
{
	*x = r->x + rand() % r->w;
	*y = r->y + rand() % r->h;
}

