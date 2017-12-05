#include <iostream>

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/stat.h> 
#include <sys/types.h> 

#include "core.h"
#include "character.h"
#include "debug.h"
#include "dungeon.h"
#include "move.h"
#include "npc.h"
#include "path.h"
#include "pc.h"
#include "room.h"
#include "turn.h"
#include "ui.h"
#include "util.h"
#include "dijkstra.h"
#include "dice.hpp"
#include "monster.hpp"

/* all global variables have been declared in core.h */
int dense = 0;

unsigned seed = SEED_UNSET;
unsigned tol = DEFAULT_TOL; // tolerance, max # of attempts to create rooms
	
char *loadp = NULL;
char *savep = NULL;

int pcx = 0; // 0 means unset
int pcy = 0; // 0 means unset

int invulnerable = 0;

int unify = 0;

int sight = 0; // 1 if display line of sight

int nummon = 10;

int nofog = 0; // 1 if no for of war

int ptime = 100000; // pause time in milliseconds

static int regenerateDungeon();

int main(int argc, char** argv)
{
  return parse_monsters();
  core_init();
  
  debug_reset();
  
  char *env = getenv("HOME");
  
  char dirpath[255];
  sprintf(dirpath, "%s/.rlg327", env);
  mkdir(dirpath, S_IRUSR | S_IWUSR | S_IXUSR);
  
  char filepath[255]; 
  sprintf(filepath, "%s/.rlg327/dungeon", env);
  
  int i;
  
  /* options */
  if (argc > 1)
    {
      for (i=1; i<argc; i++)
	{
	  if (!strcmp("--debug", argv[i]))
	    {
	      debug = 1;
	    }
	  /* required options */
	  else if (!strcmp("--load", argv[i]))
	    {
	      if (i+1<argc && argv[i+1][0]!='-')
		loadp = argv[++i];
	      else
		loadp = filepath;
	    }
	  else if (!strcmp("--save", argv[i]))
	    {
	      if (i+1<argc && argv[i+1][0]!='-')
		savep = argv[++i];
	      else
		savep = filepath;
	    }
	  else if (!strcmp("--nummon", argv[i]))
	    {
	      if (i+1<argc && argv[i+1][0]!='-')
		{
		  nummon = atoi(argv[++i]);
		  
		  if (nummon > BUFFER_SIZE)
		    {
		      fprintf(stderr, "nummon should not exceed %d\n",
			      BUFFER_SIZE);
		      return 1;
		    }
		}
	      else
		{
		  fprintf(stderr, "usage: rlg327 --nummon <number>\n");
		  return 1;
		}
	    }
	  /* for fun options */
	  else if (!strcmp("--cheat", argv[i]))
	    {
	      invulnerable = 1;
	    }
	  else if (!strcmp("--dense", argv[i]))
	    {
	      dense = 1;
	    }
	  else if (!strcmp("--hpw", argv[i]))
	    {
	      if (i+1<argc && argv[i+1][0]!='-')
		{
		  util_hpw = atoi(argv[++i]);
		}
	      else
		{
		  fprintf(stderr, "usage: rlg327 --hpw <hpw>\n");
		  return 1;
		}
	    }
	  else if (!strcmp("--help", argv[i]) || !strcmp("-h", argv[i]))
	    {
	      // display help page (README.txt)
	      util_help();
	      return 1;
	    }
	  else if (!strcmp("--nofog", argv[i]))
	    {
	      nofog = 1;
	    }
	  else if (!strcmp("--pc", argv[i]))
	    {
	      if (i+2<argc)
		{
		  pcx = atoi(argv[++i]);
		  pcy = atoi(argv[++i]);
		}
	      else
		{
		  fprintf(stderr, "usage: rlg327 --pc <x> <y>\n");
		  return 1;
		}
	    }
	  else if (!strcmp("--ptime", argv[i]))
	    {
	      if (i+1<argc)
		{
		  ptime = atoi(argv[++i]) * 1000;
		}
	      else
		{
		  fprintf(stderr, "usage: rlg327 --ptime <ptime>\n");
		  return 1;
		}
	    }
	  else if (!strcmp("--seed", argv[i]))
	    {
	      if (i+1<argc)
		{
		  seed = atoi(argv[++i]);
		}
	      else
		{
		  fprintf(stderr, "usage: rlg327 --seed <seed>\n");
		  return 1;
		}
	    }
	  else if (!strcmp("--sight", argv[i]))
	    {
	      sight = 1;
	    }
	  else if (!strcmp("--tol", argv[i]))
	    {
	      if(i+1<argc)
		{
		  tol = atoi(argv[++i]);
		}
	      else
		{
		  fprintf(stderr, "usage: rlg327 --tol <tolerance>\n");
		  return 1;
		}
	    }
	  else if (!strcmp("--unify", argv[i]))
	    {
	      if (i+1<argc && argv[i+1][0]!='-')
		{
		  unify = argv[++i][0];
		  
		  if (!(('0' <= unify && unify <= '9')
			||('a' <= unify && unify <= 'f')))
		    {
		      fprintf(stderr, "monster must be 0-9 a-f\n");
		      return 1;
		    }
		}
	      else
		{
		  fprintf(stderr, "usage: rlg327 --unify <monster>\n");
		  return 1;
		}
	    }
	  // undefined option
	  else
	    {
	      fprintf(stderr, "option %s is undefined.\n", argv[i]);
	      fprintf(stderr, "see available options using -h switch.\n");
	      return 1;
	    }
	}
    } // end of options
  
  if (!seed) 
    // if seed is unset
    srand(seed = time(NULL));
  else 
    // seed is set by -s option
    srand(seed);

  debug_log("seed: %d", seed);
  debug_log("save path: %s", savep);
  debug_log("load path: %s\n", loadp);
  
  if (loadp)
    {
      if(dungeon_load())
	{
	  fprintf(stderr, "dungeon: failed to load dungeon.\n");
	  return 1; // load may fail when file does not exist
	}
    }
  else
    {
      dungeon_generate();
    }
  
  pc = character_new(1);	
  
  turn_init();
  turn_enqueue(pc);
  
  // place NPC	
  for (i=0; i<nummon; i++)
    {
      npcs[i] = character_new(0);
      character_place(npcs[i]);
      turn_enqueue(npcs[i]);
    }
  
  // place PC	
  if (!pcx && !pcy) // if PC location is unset
    {
      character_place(pc);
    }
  else
    {
      cmap[pcy][pcx] = pc;
    }
  
  // unify all monsters to one type
  if (unify)
    {
      // unify characteristics
      char unifyc = unify >= 'a' ? unify - 'a' + 10 : unify - '0'; 
      for (i=0; i<nummon; i++)
	character_setAbil(npcs[i], unifyc);
    }
  
  // init distance map
  int pcx, pcy;
  character_getLocation(pc, &pcx, &pcy);
  dijkstra(pcx, pcy, 0);
  dijkstra(pcx, pcy, 1);
  
  // ncurses stuff
  initscr();
  start_color();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  
  // game simulation
  while (1)
    {
      ui_clearRow(DUNGEON_H+2);
      
      char buffer[80];
      if (character_isDead(pc))
	sprintf(buffer, " YOU LOST ! (press Q to quit)");
      else if (nummon == 0)
	sprintf(buffer, " YOU WON !! (press Q to quit)");
      else 
	sprintf(buffer,"%s%d MONSTERS LEFT !! %s",
		nummon < 10 ? " ":"",
		nummon,
		"(hold A to use PC AI)");
      
      mvprintw(DUNGEON_H+2, 1, "%s", buffer);
      
      debug_log("dungeon printing...");
      
      ui_printDungeon();
      
      debug_log("dungoen printed.\n");
      
      refresh();
      
      if (character_isDead(pc) || !nummon)
	{
	  char ch;
	  while ((ch=getch())!='Q' && (ch!='q'));
	  break;
	}
      
      // usleep(ptime);
      
      int quit = 0;
      
      int ch = getch();
      
      // 1 if user does some invalid action 
      int invalid = 0;
      
      int pcx, pcy;
      character_getLocation(pc, &pcx, &pcy);
      
      switch (ch)
	{
	case 'Q':
	case 'q':
	  quit = 1;
	  break;
	case 'A':
	case 'a':
	  move_pc(); // automatic
	  break;
	  /* 1.05 */
	case '7':
	case 'y':
	  invalid = move_c(pc, pcx-1, pcy-1);
	  break;
	case '8':
	case 'k':
	  invalid = move_c(pc, pcx, pcy-1);
	  break;
	case '9':
	case 'u':
	  invalid = move_c(pc, pcx+1, pcy-1);
	  break;
	case '6':
	case 'l':
	  invalid = move_c(pc, pcx+1, pcy);
	  break;
	case '3':
	case 'n':
	  invalid = move_c(pc, pcx+1, pcy+1);
	  break;
	case '2':
	case 'j':
	  invalid = move_c(pc, pcx, pcy+1);
	  break;
	case '1':
	case 'b':
	  invalid = move_c(pc, pcx-1, pcy+1);
	  break;
	case '4':
	case 'h':
	  invalid = move_c(pc, pcx-1, pcy);
	  break;
	case '>':
	  if (dungeon[pcy][pcx]=='>')
	    regenerateDungeon();
	  else
	    invalid = 1;
	  break;
	case '<':
	  if (dungeon[pcy][pcx]=='<')
	    regenerateDungeon();
	  else
	    invalid = 1;
	  break;
	case ' ':
	  break;
	case 'm':
	  ui_mList();
	  break;
	case 'S':
	  // case 's':
	  quit = 1;
	  break;
	default:
	  invalid = 1;
	  break;
	}
      
      if (ch == 'm')
	continue;
      if (ch == STAIR_UP || ch == STAIR_DOWN)
	continue;
      if (invalid)
	continue;
      
      if (quit)
	break;
      
      while (1)
	{
	  Character *c = turn_dequeue();
	  
	  if (character_isDead(c))
	    {
	      character_delete(c);
	      continue;
	    }
	  
	  turn_enqueue(c);
	  
	  if (character_isPC(c))
	    {
	      break;
	    }
	  else
	    {
	      move_npc(c);
	      if (character_isDead(pc))
		break;
	    }
	} // end of NPC turn
    } // end of game simulation
  
  endwin();
  
  if (savep)
    {
      dungeon_save();
    }
  
  turn_delete();
  
  dungeon_delete();
  
  npc_deleteAll();
  
  character_delete(pc);
  
  return 0;
}

static int regenerateDungeon()
{
  int i;
  
  dungeon_delete();
  
  npc_deleteAll();
  
  dungeon_generate();
  
  pc_clearSeenDungeon();
  
  // reset turn	
  turn_delete();	
  turn_init();
  character_setTurn(pc, 0);
  
  turn_enqueue(pc);
  
  // reset monsters
  nummon = 10; 
  for (i=0; i<nummon; i++)
    {
      npcs[i] = character_new(0);
      character_place(npcs[i]);
      turn_enqueue(npcs[i]);
    }
  
  character_place(pc);
  
  int pcx, pcy;
  character_getLocation(pc, &pcx, &pcy);
  
  // init distance map
  dijkstra(pcx, pcy, 0);
  dijkstra(pcx, pcy, 1);
  
  return 0;
}

