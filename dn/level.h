#ifndef LEVEL_H
#define LEVEL_H

class Level {
  int levelIndex;
  byte tileAt(int x, int y);
public:
  Color palette[4][16];
  Image *background;
  
  void load(int levelIndex);
  bool collisionAt(int x, int y, int width, int height);
  bool lavaAt(int x, int y, int width, int height);
  int getLevelIndex();
};

#endif
