#ifndef PLAYER_H
#define PLAYER_H

#include "level.h"

class Player {
  int x, y;
  int velX, velY;
  int wallJumpDelay;
  bool touchingGround, facingLeft, touchingRightWall, touchingLeftWall;

  void internalUpdate(Level &l, bool firstUpdate);
public:
  void resetPosition(Level &l);
  int getX(); 
  int getY(); 
  void draw();
  void update(Level &l);
  
  int getWidth() { return 4; }
  int getHeight() { return 4; }
};

#endif
