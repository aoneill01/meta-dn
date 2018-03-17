#include <Gamebuino-Meta.h>
#include "sprites.h"
#include "player.h"
#include "level.h"

Color palette[16] = {
    Color::black,
    Color::darkblue,
    Color::purple,
    Color::green,
    Color::brown,
    Color::darkgray,
    Color::gray,
    Color::white,
    Color::red,
    Color::orange,
    Color::yellow,
    Color::lightgreen,
    Color::lightblue,
    Color::blue,
    Color::pink,
    Color::beige,
  };

Player player;
Level level;
const int foo = 4;

void setup() {
  // Initialize Gamebuino library
  gb.begin();

  SerialUSB.begin(9600);
  // Load the first level
  level.load(0);
  
  gb.display.drawImage(0, 0, *level.background);

  // Set color palette
  gb.display.colorIndex = level.palette[0];

  player.resetPosition(level);
}

void loop() {
  // Ready to update next frame?
  while (!gb.update());
  
  // drawCpu();
  
  gb.display.colorIndex = level.palette[(gb.frameCount >> 3) % 4];

  clearPlayer();

  player.update(level);
  player.draw();
  
  recordFrame();
}

void clearPlayer() {
  int gridX = player.getX() / 4;
  int gridY = player.getY() / 4;

  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 2; y++) {
      gb.display.drawImage((gridX + x) * 4, (gridY + y) * 4, *level.background, (gridX + x) * 4, (gridY + y) * 4, 4, 4);
    }
  }
}

void drawCpu() {
  gb.display.setColor((ColorIndex)14);
  gb.display.fillRect(0, 0, 17, 7);
    
  gb.display.setCursor(1, 1);
  gb.display.setColor((ColorIndex)15);

  gb.display.print(gb.getCpuLoad());
  gb.display.print('%');
}

int frameId = 0;
void recordFrame() {
  char filename[256];
  snprintf(filename, sizeof filename, "frame%04d.bmp", frameId);
  gb.display.save(filename);
  frameId++;
}

Color hsvToRgb565(uint8_t h, uint8_t s, uint8_t v)
{
    uint8_t r, g, b;
    uint8_t region, remainder, p, q, t;

    if (s == 0)
    {
        r = v;
        g = v;
        b = v;
        return (Color)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
    }

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            r = v; g = t; b = p;
            break;
        case 1:
            r = q; g = v; b = p;
            break;
        case 2:
            r = p; g = v; b = t;
            break;
        case 3:
            r = p; g = q; b = v;
            break;
        case 4:
            r = t; g = p; b = v;
            break;
        default:
            r = v; g = p; b = q;
            break;
    }

    return (Color)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}
