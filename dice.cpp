#include <iostream>
#include <stdlib.h>

#include "dice.hpp"

Dice::Dice(){}

Dice::Dice(int b, int d, int s){
  base = b;
  dice = d;
  sides = s;
}

int Dice::roll(){
  int i, roll;
  for(i = dice, roll = base; i > 0; i--, roll+=(rand()%sides));
  return roll;
}

std::ostream &operator<<(std::ostream &os, const Dice &d){
  os << d.base << '+' << d.dice << 'd' << d.sides;
  return os;
}
