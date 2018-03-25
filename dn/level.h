#ifndef LEVEL_H
#define LEVEL_H

#define LEVEL_ROWS 32
#define LEVEL_COLS 40 

class Level {
  int levelIndex = -1;
  byte tiles[LEVEL_ROWS][LEVEL_COLS];
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
