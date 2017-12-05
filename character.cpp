#include "character.h"
#include "pc.h"
#include "npc.h"

__Character::__Character()
{
  dead = false;
  turn = 0;
}

__Character::~__Character()
{
}

void __Character::getLocation(int *x, int *y)
{
  *x = this->x;
  *y = this->y;
}

void __Character::setLocation(int x, int y)
{
  this->x = x;
  this->y = y;
}

Character *cmap[DUNGEON_H][DUNGEON_W];

static int getNPCIndex(Character *c)
{
  int i;
  for(i=0; i<nummon; i++)
    {
      if (npcs[i]==c)
	return i;
    }
  return -1;
}

#ifdef __cplusplus
extern "C" {
#endif
  
#include <stdio.h>
#include <stdlib.h>
  
#include "debug.h"
#include "util.h"
  
  Character *character_new(int isPC)
  {
    if (isPC)
      return (Character *) new __PC;
    else
      return (Character *) new __NPC;
  }
  
  int character_delete(Character *c)
  {
    if (character_isPC(c))
      delete (__PC *) c;
    else
      {
	int index = getNPCIndex(c);
	npcs[index] = npcs[--nummon];
	delete (__NPC *) c;
      }
    return 0;
  }
  
  int character_isDead(Character *c)
  {
    return ((__Character *)c)->isDead();
  }
  
  int character_isPC(Character *c)
  {
    return ((__Character *)c)->isPC();
  }
  
  int character_getAbil(Character *c)
  {
    if (character_isPC(c))
      return -1;
    else
      return ((__NPC *)c)->getAbil();
  }
  
  int character_getLocation(Character *c, int *x, int *y)
  {
    ((__Character *)c)->getLocation(x, y);
    return 0;
  }
  
  int character_getMemPCLocation(Character *c, int *x, int *y)
  {
    ((__NPC *)c)->getMemPCLocation(x, y);
    return 0;
  }
  
  int character_getSpeed(Character *c)
  {
    return ((__Character *)c)->getSpeed();
  }
  
  char character_getSymbol(Character *c)
  {
    return ((__Character *)c)->getSymbol();
  }
  
  int character_getTurn(Character *c)
  {
    return ((__Character *)c)->getTurn();
  }
  
  int character_setAbil(Character *c, int abil)
  {
    if (character_isPC(c))
      return -1;
    else
      ((__NPC *)c)->setAbil(abil);
    
    return 0;
  }
  
  int character_setDead(Character *c)
  {
    ((__Character *)c)->setDead();
    return 0;
  }
  
  int character_setLocation(Character *c, int x, int y)
  {
    if (character_isPC(c))
      ((__PC *)c)->setLocation(x, y);
    else
      ((__NPC *)c)->setLocation(x, y);
    
    return 0;
  }
  
  int character_setMemPCLocation(Character *c, int x, int y)
  {
    ((__NPC *)c)->setMemPCLocation(x, y);
    return 0;
  }
  
  int character_setTurn(Character *c, int turn)
  {
    ((__Character *)c)->setTurn(turn);
    return 0;
  }
  
  int character_place(Character *cp)
  {
    while(1)
      {
	int r = rand() % DUNGEON_H;
	int c = rand() % DUNGEON_W;
	
	if (!hardness[r][c])
	  {
	    cmap[r][c] = cp;
	    
	    character_setLocation(cp, c, r);
	    break;
	  }
      }
    
    if (character_isPC(cp))
      pc = cp;
    
    return 0;
  }
  
#ifdef __cplusplus
}
#endif

