#ifndef DICE_H
#define DICE_H

#include <iostream>

class Dice{
  int base;
  int dice;
  int sides;
public:
  friend std::ostream &operator<<(std::ostream&, const Dice&);
  int roll();
  Dice();
  Dice(int, int, int);
  inline int get_dice() const {return dice;}
  inline int get_base() const {return base;}
  inline int get_sides() const {return sides;}
};

#endif
