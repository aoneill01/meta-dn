#ifndef LEVEL_H
#define LEVEL_H

class Level {
  bool tileAt(int x, int y);
public:
  void draw();
  void drawAt(int gridX, int gridY);
  bool collisionAt(int x, int y, int width, int height);
};

#endif
