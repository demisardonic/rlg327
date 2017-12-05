#ifndef DUNGEON_HEADER
#define DUNGEON_HEADER

/* 1 if display line of sight */
extern int sight;

/* delete the dungeon */
int dungeon_delete();

/* compute the size of the file that stores the dungeon */
int dungeon_filesize();

/* fill the mutable part of the dungeon with given char */
int dungeon_fill(char);

/* print the dungeon to stream */
int dungeon_print();

/* generate random dungeon */
int dungeon_generate();

/* 1 if the given location is visible */
int dungeon_isVisible(int x, int y);

/* 1 if there is a line of sight between two points */
int dungeon_lineOfSight(int, int, int, int);

/* load dungeon using --load <file> */
int dungeon_load();

/* save dungeon using --save <file> */
int dungeon_save();

#endif

