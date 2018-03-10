#include <Gamebuino-Meta.h>
#include "sprites.h"
#include "player.h"
#include "level.h"

Color palette[16];
Color cycle[6];

Player player;
Level level;

void setup() {
  // Initialize Gamebuino library
  gb.begin();

  int offsetX = 0;
  int offsetY = 0;
  for (int i = 0; i < (160 * 128) / 4; i++) {
    background.setFrame(i);
    gb.display.drawImage(offsetX, offsetY, background);
    offsetX += 4; 
    if (offsetX >= 160) {
      offsetX = 0;
      offsetY += 4;
    }
  }

  gb.display.colorIndex = palette;

  for (int i = 0; i < 6; i++) {
    cycle[i] = hsvToRgb565(128, 5, (255 - 15) + i * 3);
  }

  palette[6] = WHITE;
  palette[7] = GRAY;
  palette[8] = BLACK; // Player
  palette[13] = RED;
  palette[14] = WHITE;
  palette[15] = BEIGE;

  player.resetPosition();

  level.draw();
}

void loop() {
  // Ready to update next frame?
  while (!gb.update());
  
  drawCpu();
  
  cycleBackgroundColors();

  clearPlayer();

  player.update(level);
  player.draw();
  
  // recordFrame();
}

void cycleBackgroundColors() {
  for (int i = 0; i < 6; i++) {
    palette[5 - i] = cycle[((gb.frameCount >> 1) + i) % 6];
  }
}

void clearPlayer() {
  int gridX = player.getX() / 4;
  int gridY = player.getY() / 4;

  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 2; y++) {
      background.setFrame((gridY + y) * (160 / 4) + gridX + x);
      gb.display.drawImage((gridX + x) * 4, (gridY + y) * 4, background);
      
      level.drawAt(gridX + x, gridY + y);
    }
  }
}

void drawCpu() {
  gb.display.setColor((ColorIndex)14);
  gb.display.fillRect(1, 1, 16, 6);
    
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
