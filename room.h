#ifndef ROOM_H
#define ROOM_H

typedef struct
{
	unsigned char x, y, w, h;
} Room;

/* all the rooms that have been added to the dungeon */
extern Room *rooms[BUFFER_SIZE];

/* number of rooms */
extern int roomc;

/* get the centroid of the room */
void room_centroid(int * x, int * y, Room *);

/* determine if two rooms are adjacent or overlap */
int room_collide(Room *, Room *);

/* 1 if room contains the given point */
int room_contains(Room *, int x, int y);

/* create a new room with random location and size */
Room* room_new();

/* delete a room */
int room_delete(Room*);

/* 1 if two points are in the same room */
int room_isInSameRoom(int x1, int y1, int x2, int y2);

/* paint the room on the dungeon */
void room_paint(Room*);

void room_randomLocation(int *x, int *y, Room *r);

#endif

