#include <ncurses.h>
#include <stdlib.h>

#include "character.h"
#include "dungeon.h"
#include "ui.h"

static void getPositionStr(char buf[50], Character *c)
{
	int i;
	for(i=0; i<50; i++) buf[i]=' ';

	int cx, cy, pcx, pcy;
	character_getLocation( c, & cx, & cy);
	character_getLocation(pc, &pcx, &pcy);

	int dx = cx - pcx;
	int dy = cy - pcy;
	
	if (dx>0)
		sprintf(buf +  5, "%02d east             ",  dx);
	else
		sprintf(buf +  5, "%02d west             ", -dx);
	if (dx==0)
		sprintf(buf +  5, "00         ");
	if (dy>0)
		sprintf(buf + 15,          "%02d south  |",  dy);
	else
		sprintf(buf + 15,          "%02d north  |", -dy);
	if (dy==0)
		sprintf(buf + 15, "00        |");
}

static int compareNPC(const void *a, const void *b)
{
	int ax, ay, bx, by, pcx, pcy;

	character_getLocation(*(Character **)a, &ax, &ay);
	character_getLocation(*(Character **)b, &bx, &by);
	character_getLocation(pc, &pcx, &pcy);
	
	return (ABS((ax-pcx))+ABS((ay-pcy)))
	     - (ABS((bx-pcx))+ABS((by-pcy)));
}

static void mListFrom(int start)
{
	int i;
	char buf[50];

	mvprintw(0, 0, "Listing monster %d to %d out of %d "
	"(press ESC or Q to quit)", 
	start+1, MIN((start+21), nummon), nummon);

	qsort(npcs, nummon, sizeof(*npcs), compareNPC);

	for(i=start; i<MIN((start+21), nummon); i++)
	{
		Character *c = npcs[i];
		getPositionStr(buf, c);
		
		mvprintw(i-start+1, 1, "  %s", buf);

		int speed = character_getSpeed(c);

		int color = 0;
		if (100/speed >= 15)
			init_pair(color = 2, COLOR_WHITE, COLOR_BLACK);
		else if (100/speed >= 10)
			init_pair(color = 3, COLOR_GREEN, COLOR_BLACK);
		else if (100/speed >= 7)
			init_pair(color = 4, COLOR_CYAN, COLOR_BLACK);
		else if (100/speed >= 5)
			init_pair(color = 5, COLOR_MAGENTA, COLOR_BLACK);

		attron(COLOR_PAIR(color));
		mvprintw(i-start+1, 1, "%c", character_getSymbol(c));
		attroff(COLOR_PAIR(color));
	}
	mvprintw(MIN((start+21), nummon)-start+1, 1, 
	"---------------------------+");
}

int ui_clearRow(int row)
{
	int i;
	for(i=0; i<80; i++)
		mvprintw(row, i, " ");
	
	return 0;
}

int ui_printDungeon()
{
	return dungeon_print();
}

int ui_mList()
{
	int from = 0;

	mListFrom(from);
	refresh();

	while(1)
	{
		int ch = getch();
		
		int quit = 0;
		
		switch(ch)
		{
			case 'Q':
			case 'q':
			case 27: // ESC
				quit = 1;
				break;
			case KEY_DOWN:
				if (from>21-nummon && from<nummon-21) from++;
				mListFrom(from);
				refresh();
				break;
			case KEY_UP:
				from = MAX((from-1), 0);
				mListFrom(from);
				refresh();
				break;
		}
		if (quit)
			break;
	}
	
	int i;
	for(i=0; i<80; i++)
	{
		mvprintw(0,  i, " ");
		mvprintw(22, i, " ");
	}

	return 0;
}

