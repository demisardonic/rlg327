#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

char util_base62(int dist)
{
	if(dist<10)
		return '0'+dist;
	else if(dist>=10 && dist<36)
		return 'a'+(dist-10);
	else if(dist>=36 && dist<62)
		return 'A'+(dist-36);
	
	return -1;
}

void util_help()
{
	FILE* fp = fopen("MANUAL", "r");
	
	if(!fp)
	{
		fprintf(stderr, "README.txt not found\n");
		return;
	}
		
	const int buf_size = 80;

	char buf[buf_size];

	// skip lines before user manual
	while(1)
	{
		fgets(buf, buf_size, fp);
		if(*buf=='/' && *(buf+1)=='/')
			break;
	}
	
	// ncurses stuff
	initscr();
	start_color();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	
	int r = 0, c = 0;
	char buffer[255][80];
	while(fgets(buffer[r++], 80, fp));
	fclose(fp);

	int rmax = r;

	int from = 0;

	while(1)
	{
		clear();

		char tmp[80];
		char *p;

		for (r=0; r<22; r++)
		{
			p = tmp;

			for (c=0; c<80; c++)
			{
				if ('\t'==buffer[from+r][c])
				{
					sprintf(p, "    ");
					p+=4;
				}
				else *p++ = buffer[from+r][c];
			}
			mvprintw(r, 0, "%s", tmp);
		}

		int color;
		init_pair(color = 9, COLOR_GREEN, COLOR_BLACK);

		attron(COLOR_PAIR(color));

		mvprintw(24-1, 0, 
		"Use arrow keys to scroll up and down. Press Q to quit.");

		attroff(COLOR_PAIR(color));

		refresh();

		int ch = getch();
		if (ch=='Q' || ch=='q') break;
		if (ch==KEY_DOWN) 
			if (from+22<rmax) 
				from++;
		if (ch==KEY_UP)   
			if (from>0)
				from--;
	}
		
	endwin();
}

unsigned int util_reverse(unsigned int i)
{
	int r = 0;
	r |= (0x000000ff & (i>>24));
	r |= (0x0000ff00 & (i>>8));
	r |= (0x00ff0000 & (i<<8));
	r |= (0xff000000 & (i<<24));
	
	return r;
}

int util_rnum(int from, int to)
{
	return from + rand() % (to - from + 1);
}

int util_hpw = 0; // weight = hardness / hpw + 1

int util_weight(int hardness)
{
	if (util_hpw) // if hardness per weight is defined
		return hardness / util_hpw + 1;
	
	if (0 <= hardness && hardness <= 84)
		return 1;
	else if ( 85 <= hardness && hardness <= 170)
		return 2;
	else if (171 <= hardness && hardness <= 254)
		return 3;
	else
		return -1;
}

