#ifndef PLAYER_H
#define PLAYER_H

class Player {
  int x, y;
  int velX, velY;
public:
  void move(int x, int y);
  int getX(); 
  int getY(); 
  void draw();
  void update();
};

#endif
