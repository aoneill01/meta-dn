#ifndef PLAYER_H
#define PLAYER_H

#include "level.h"

class Player {
  int x, y;
  int velX, velY;
  bool touchingGround, facingLeft, touchingWall;

  void internalUpdate(Level &l);
public:
  void resetPosition();
  int getX(); 
  int getY(); 
  void draw();
  void update(Level &l);
  
  int getWidth() { return 4; }
  int getHeight() { return 4; }
};

#endif
