#include <iostream>
#include "controls.h"


controls::controls(){
  std::cout<<"hello world";
}

void controls::render(){
  controls::playerX+=0.1;
  std::cout<<"rendering physics: "<<playerX<<std::endl;
}

float controls::getPlayerX(){
  return playerX;
}
float controls::getPlayerY(){
  return playerY;
}
float controls::getPlayerZ(){
  return playerZ;
}
