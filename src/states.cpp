// Author: Lin
// Date: 2025-12-21 15:03:48
// Description:

#include "src/states.hpp"
#include <QDebug>
/*
    0   1   2   3   4   5   6  7  8  9   10    11    12  13    14     15     16
  ATK ATK% HP HP% DEF DEF% CR CD EM ER Anemo Pyro Hydro Geo Electro Dendro Cryo
*/
States::States(QObject *parent): 
  QObject(parent), 
  MainStates{0b0100, 0b01, 0b010, 0b010, 0b010}, 
  SubStates(5,0), 
  MainMasks{0b0100, 0b01, 0b01100101010, 0b011111110100101010, 0b0111101010}, 
  SubMasks{0b01111111011, 0b01111111110, 0b01111111111, 0b01111111111, 0b01111111111}, 
  states{
    2,3,6,7,8,
    0,1,6,7,8,
    3,2,6,7,8,
    12,3,6,7,8,
    7,2,3,6,8
  }
{
  restState();
}

uT States::relicState(int i) const {
  // it is the main stat
  if( i%5==0 ){
    return ~(MainStates[i/5] | SubStates[i/5]) & MainMasks[i/5];
  } else {
    return ~(MainStates[i/5] | SubStates[i/5]) & SubMasks[i/5];
  }
}

void States::setRelicState(int i, uT v) {
  // first check which stat is updated
  int idx=i/5;
  // recalculate MainStates or SubStates
  if(i%5==0){
    MainStates[idx]=(1<<v);
  }else {
    SubStates[idx]=(SubStates[idx]&~(1<<states[i]))|(1<<v);
  }
  // update the states
  states[i]=v;
}

uT States::getState(int i) {
  return states[i];
}

void States::restState(){
  // reset MainState
  for(int i=0; i<5; ++i)MainStates[i]=(1<<states[5*i]);
  // reset SubState
  for(int i=0; i<5; ++i){
    SubStates[i]=0;
    for(int j=1; j<5; ++j){
      SubStates[i]|=(1<<states[5*i+j]);
    }
    qDebug() << SubStates[i];
  }
}