#include <stdio.h>
#include <stdlib.h>

#include "npc.h"

__NPC::__NPC() : __Character()
{
  pc = false;
  
  abil = rand() % 16;
  speed = 5 + rand() % 16;
  
  // get symbol
  char symb[2];
  sprintf(symb, "%x", abil);
  symbol = symb[0];
  
  mempcx = 0;
  mempcy = 0;
}

__NPC::~__NPC()
{
}

/* all monsters */
Character *npcs[BUFFER_SIZE];

void __NPC::getMemPCLocation(int *x, int *y)
{
  *x = mempcx;
  *y = mempcy;
}

void __NPC::setMemPCLocation(int x, int y)
{
  mempcx = x;
  mempcy = y;
}


int npc_deleteAll()
{
  int i;
  for(i = 0; i < nummon; i++)
    character_delete(npcs[i]);	
  
  return 0;
}



