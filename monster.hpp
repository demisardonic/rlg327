#ifndef MONSTER_H
#define MONSTER_H

#include <string>

#include "dice.hpp"
#include "npc.h"

class Monster{
  std::string name;
  std::string desc;
  int abil;
  int color;
  Dice speed;
  Dice hp;
  Dice dam;
  char symb;
 public:
  Monster(std::string, std::string, int, int, Dice, Dice, Dice, char);
  inline std::string get_name() const {return name;}
  inline std::string get_desc() const {return desc;}
  inline int get_abil() const {return abil;}
  inline int get_color() const {return color;}
  inline Dice get_speed() const {return speed;}
  inline Dice get_hp() const {return hp;}
  inline Dice get_dam() const {return dam;}
  inline char get_symb() const {return symb;}

  __NPC generate();
};

int parse_monsters();

#endif
