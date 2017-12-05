#include <string>
#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <stdlib.h>

#include "monster.hpp"
#include "npc.h"

#define _NAME 0x01
#define _DESC 0x02
#define _SYMB 0x04
#define _COL 0x08
#define _SPE 0x10
#define _ABIL 0x20
#define _HP 0x40
#define _DAM 0x80

Monster::Monster(std::string name, std::string desc, int abil, int color, Dice speed, Dice hp, Dice dam, char symb){
  this->name = name;
  this->desc = desc;
  this->abil = abil;
  this->color = color;
  this->speed = speed;
  this->hp = hp;
  this->dam = dam;
  this->symb = symb;
}

static std::string color_text(int c){
  switch(c){
  case COLOR_BLACK:
    return "BLACK";
  case COLOR_RED:
    return "RED";
  case COLOR_GREEN:
    return "GREEM";
  case COLOR_YELLOW:
    return "YELLOW";
  case COLOR_BLUE:
    return "BLUE";
  case COLOR_MAGENTA:
    return "MAGENTA";
  case COLOR_CYAN:
    return "CYAN";
  case COLOR_WHITE:
    return "WHITE";
  default:
    return "NULL";
  } 
}
static int color_int(std::string s){
  if(!s.compare("BLACK"))
    return COLOR_BLACK;
  else if(!s.compare("RED"))
    return COLOR_RED;
  else if(!s.compare("GREEN"))
    return COLOR_GREEN;
  else if(!s.compare("YELLOW"))
    return COLOR_YELLOW;
  else if(!s.compare("BLUE"))
    return COLOR_BLUE;
  else if(!s.compare("MAGENTA"))
    return COLOR_MAGENTA;
  else if(!s.compare("CYAN"))
    return COLOR_CYAN;
  else if(!s.compare("WHITE"))
    return COLOR_WHITE;
  else{
    std::cout<<std::endl<<s;
    return -1;
  }
}
/*
  static std::string abils(int a){
  return a ? "ABILS YES" : "ABILS NO";
}
static int abils_int(std::string s){
  unsigned int i, word_begin, word_len;
  int abil = 0;
  for(i = 0, word_begin = 0; i < s.length(); i++){
    if(s[i] == ' '){
      word_len = i - word_begin;
      std::cout << std::endl << "Word: " << s.substr(word_begin, word_len) << std::endl;
      word_begin = i + 1;
    }
  }
  return abil;
  
  }*/
int parse_monsters(){
  std::string name, desc;
  char symb;
  Dice speed, dam, hp;
  //To be changed
  std::string abil;
  int color;
  unsigned char loaded = 0;
  int base, sides, dice;
  std::string file, s;
  file += getenv("HOME");
  file += "/.rlg327/monster_desc.txt";
  std::ifstream f(file.c_str());
  std::string::iterator iter;
  if(!f.is_open()){
    return 0;
  }
  std::getline(f,s);
  if(s.compare("RLG327 MONSTER DESCRIPTION 1")){
    return 0;
  }
  while(std::getline(f, s)){
    if(!s.compare("BEGIN MONSTER")){
      loaded &= 0x00;
      while(std::getline(f, s)){
	if(!s.compare("END") && (loaded & 0xff) == 0xff){
	  std::cout << name << std::endl;
	  std::cout<< desc << std::endl;
	  std::cout << symb << std::endl;
	  std::cout << color_text(color) << std::endl;
	  std::cout << speed << std::endl;
	  //std::cout << abils(abil) << std::endl;
	  std::cout << abil << std::endl;
	  std::cout << hp << std::endl;
	  std::cout << dam << std::endl;
	  std::cout << std::endl;
	  
	  break;
	}else if(!s.compare("DESC") && !(loaded & _DESC)){
	  loaded |= _DESC;
	  desc = "";
	  while(std::getline(f, s)){
	    if(!s.compare(".")){
	      break;
	    }
	    desc += s;
	    desc += "\n";
	  }
	  desc = desc.substr(0, desc.length()-1);
	}else if(!s.compare("BEGIN MONSTER")){
	  loaded = 0;
	  break;
	}else if(!s.compare(0, 4, "NAME") && !(loaded & _NAME)){
	  name = s.substr(5, s.length());
	  loaded |= _NAME;
	}else if(!s.compare(0, 5, "SPEED") && !(loaded & _SPE)){
	  sscanf(s.substr(5, s.length()).c_str(),"%d+%dd%d", &base, &dice, &sides);
	  speed = Dice(base, dice, sides);
	  loaded |= _SPE;
	}else if(!s.compare(0, 2, "HP") && !(loaded & _HP)){
	  sscanf(s.substr(2, s.length()).c_str(),"%d+%dd%d", &base, &dice, &sides);
	  hp = Dice(base, dice, sides);
	  loaded |= _HP;
	}else if(!s.compare(0, 3, "DAM") && !(loaded & _DAM)){	
	  sscanf(s.substr(3, s.length()).c_str(),"%d+%dd%d", &base, &dice, &sides);
	  dam = Dice(base, dice, sides);
	  loaded |= _DAM;
	}else if(!s.compare(0, 4, "SYMB") && !(loaded & _SYMB)){
	  sscanf(s.c_str(),"SYMB %c", &symb);
	  loaded |= _SYMB;
	}else if(!s.compare(0, 5, "COLOR") && !(loaded & _COL)){
	  color = color_int(s.substr(6, s.length()));
	  loaded |= _COL;
	}else if(!s.compare(0, 4, "ABIL") && !(loaded & _ABIL)){
	  //abil = abils_int(s.substr(5, s.length()));
	  abil = s.substr(5, s.length());
	  loaded |= _ABIL;
	}else{
	  loaded &= 0x00;
	  break;
	}

      }
    }
    //

  }
  
  return 1;
}
