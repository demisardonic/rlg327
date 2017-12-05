#ifndef CORE_H
#define CORE_H

/* core settings for dungeon */

#define ROCK ' ' /* char for rock */
#define ROOM '.' /* char for room */
#define PATH '#' /* char for path */
#define STAIR_DOWN '>'
#define STAIR_UP '<'

#define DUNGEON_W 80 /* dungeon width */
#define DUNGEON_H 21 /* dungeon height */

#define ROOM_MIN_W 3 /* room min width */
#define ROOM_MIN_H 2 /* room min height */

#define ROOM_MAX_W 15 /* room max width */
#define ROOM_MAX_H 10 /* room max height */

#define IMMUTABLE 255
#define MUTABLE_SIZE ((DUNGEON_W-2)*(DUNGEON_H-2))

#define BUFFER_SIZE (1<<8)

#define DEFAULT_TOL (1<<5) /* default tolerance value */

#define SEED_UNSET 0 /* let 0 denote unset seed value */

#define MARKER "RLG327"

#define ABS(x)    (x>0?x:-x)
#define MAX(x, y) (x>y?x:y)
#define MIN(x, y) (x<y?x:y)

/* 1 if paths are dense, 0 if paths are sparse */
extern int dense;

/* load path */
extern char* loadp;
/* save path */
extern char* savep;

/* 1 if PC invulnerable */
extern int invulnerable;

/* number of monsters */
extern int nummon;

/* 1 if no fog of war */
extern int nofog;

/* seed for generating random number */
extern unsigned int seed;
/* max number of attempts to create new rooms */
extern unsigned int tol;

extern char dungeon[DUNGEON_H][DUNGEON_W];

extern unsigned char hardness[DUNGEON_H][DUNGEON_W];

/* initialize all the parameters declared here */
int core_init();

#endif

