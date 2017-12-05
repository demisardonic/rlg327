#ifndef PC_H
#define PC_H

#include "character.h"

#ifdef __cplusplus

class __PC : public __Character
{
	public:
		__PC();
		~__PC();

		char *seenDungeon;
	
		void clearSeenDungeon();

		void setLocation(int x, int y);
};

#endif // c++


char *pc_getSeenDungeon();

int pc_clearSeenDungeon();


#endif

