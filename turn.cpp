#include <stdio.h>

#include "core.h"
#include "debug.h"
#include "heap.h"
#include "character.h"

static Heap *h;

static int compare(const void *cv, const void *dv)
{
	Character *c = (Character *) cv;
	Character *d = (Character *) dv;
	
	int cturn = character_getTurn(c);
	int dturn = character_getTurn(d);

	if (cturn == dturn)
		return (!character_isPC(c)) - 
		       (!character_isPC(d)); // PC has higher priority
	else
		return cturn - dturn;
}

int turn_init()
{
	h = heap_new(compare);
	return 0;
}

int turn_delete()
{
	heap_delete(h);
	return 0;
}

int turn_enqueue(Character *c)
{
	int turn = character_getTurn(c);
	int speed = character_getSpeed(c);
	/*
	   debug_log("%c's turn = %d\tspeed = %d\t", 
	   character_getSymbol(c), turn, speed);
	 */
	turn += (100/speed);
	character_setTurn(c, turn);
	/*
	   debug_log("%c's next turn = %d\n",
	   character_getSymbol(c), turn);
	 */
	heap_insert(h, c);
	return 0;
}

Character *turn_dequeue()
{
	return heap_extract(h);
}


