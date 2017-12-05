#ifndef CHARACTER_H
#define CHARACTER_H

#include "core.h"

#ifdef __cplusplus

class __Character
{
 protected:
  int x, y;
  bool dead;
  bool pc; 
  int speed;
  char symbol;
  int turn;
  
 public:
  __Character();
  ~__Character();
  
  inline bool isDead() {return dead;}
  inline bool isPC() {return pc;}
  
  void getLocation(int *, int *);
  
  inline int getX() {return x;}
  inline int getY() {return y;}
  
  inline int getTurn() {return turn;}
  inline int getSpeed() {return speed;}
  inline char getSymbol() {return symbol;}
  
  inline void setDead() {dead = true;}
  
  void setLocation(int x, int y);
  
  inline void setTurn(int turn) {this->turn = turn;}
};

#endif

typedef void Character;

extern Character *pc;
extern Character *npcs[];

extern Character *cmap[DUNGEON_H][DUNGEON_W];

#ifdef __cplusplus
extern "C" {
#endif
  
  Character *character_new(int isPC);
  
  int character_delete(Character *);
  
  int character_isDead(Character *);
  
  int character_isPC(Character *);
  
  int character_getAbil(Character *);
  
  int character_getLocation(Character *, int *, int *);
  
  int character_getMemPCLocation(Character *, int *, int *);
  
  int character_getSpeed(Character *);
  
  char character_getSymbol(Character *);
  
  int character_getTurn(Character *);
  
  int character_setAbil(Character *, int abil);
  
  int character_setDead(Character *);
  
  int character_setLocation(Character *, int x, int y);
  
  int character_setMemPCLocation(Character *, int x, int y);
  
  int character_setTurn(Character *, int turn);
  
  int character_place(Character *);
  
#ifdef __cplusplus
}
#endif

#endif

