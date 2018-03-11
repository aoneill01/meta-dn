#ifndef LEVEL_H
#define LEVEL_H

class Level {
  byte tileAt(int x, int y);
public:
  bool collisionAt(int x, int y, int width, int height);
  bool lavaAt(int x, int y, int width, int height);
};

#endif
