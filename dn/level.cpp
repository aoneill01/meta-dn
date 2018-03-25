#include <Gamebuino-Meta.h>
#include "level.h"

#define TILE_SIZE 4
#define END_OF_FLASH (256 * 1024)
#define BACKGROUND_SIZE ((160 * 128 / 2) + 8)
#define BACKGROUNDS_IN_FLASH 8
#define BACKGROUND_START_ADDR (END_OF_FLASH - BACKGROUNDS_IN_FLASH * BACKGROUND_SIZE)

void Level::load(int levelIndex) {
  SerialUSB.printf("Loading level %d\n", levelIndex);
  
  char filename[256];
  const int blockSize = 64;
  uint8_t header[] = {0, 160, 128, 1, 0, 0, 0xFF, 1};

  snprintf(filename, sizeof filename, "levels/%03d.dnl", levelIndex);
  File levelFile = SD.open(filename, FILE_READ);

  // Skip header. TODO: Validate header and version.
  levelFile.read();
  levelFile.read();
  levelFile.read();

  levelFile.read(&tiles, LEVEL_ROWS * LEVEL_COLS);

  for (int i = 0; i < 4; i++) {
    levelFile.read(&palette[i], 2 * 16);  
  }

  // TODO: When we have more levels than can fit in flash, detect when flash needs to be rewritten
  if (this->levelIndex == -1) {
    uint8_t *backgroundData = (uint8_t*)BACKGROUND_START_ADDR;
    gb.bootloader.flash_delete(BACKGROUND_START_ADDR);
    
    for (int l = 0; l < BACKGROUNDS_IN_FLASH; l++) {
      SerialUSB.printf("Loading background for level %d\n", l);
      
      snprintf(filename, sizeof filename, "levels/%03d.dnl", l);
      
      levelFile = SD.open(filename, FILE_READ);

      if (levelFile) {
        // Skip to right location. 
        levelFile.seek(LEVEL_ROWS * LEVEL_COLS + 3 + 4 * 2 * 16);
        
        gb.bootloader.flash_write(8, (uint32_t*)header, (uint32_t*)backgroundData);
      
        backgroundData += 8;
      
        for (int i = 0; i < (BACKGROUND_SIZE - 8) / blockSize; i++) {
          uint8_t next[blockSize];
          levelFile.read(&next, blockSize);
      
          gb.bootloader.flash_write(blockSize / 4, (uint32_t*)next, (uint32_t*)backgroundData);
          backgroundData += blockSize;
        }
      }
    }
  }

  this->levelIndex = levelIndex;
    
  if (!background) {
    background = new Image((uint8_t*)((BACKGROUND_START_ADDR + BACKGROUND_SIZE * levelIndex) + 1));
  }
  else {
    background->init((uint8_t*)((BACKGROUND_START_ADDR + BACKGROUND_SIZE * levelIndex) + 1));
  }
}

bool Level::collisionAt(int x, int y, int width, int height) {
  if (tileAt(x, y + height - 1)) return true;
  if (tileAt(x + width - 1, y + height - 1)) return true;
  if (tileAt(x + width - 1, y)) return true;
  if (tileAt(x, y)) return true;

  return false;
}

bool Level::lavaAt(int x, int y, int width, int height) {
  if (tileAt(x + width / 2, y) == 2) return true;
  if (tileAt(x + width / 2, y + height - 1) == 2) return true;
  if (tileAt(x, y + height / 2) == 2) return true;
  if (tileAt(x + width - 1, y + height / 2) == 2) return true;

  return false;
}

byte Level::tileAt(int x, int y) {
  int gridX = x / TILE_SIZE;
  int gridY = y / TILE_SIZE;

  if (gridY >= LEVEL_ROWS) return 0;
  // Solid outside of map
  if (gridX < 0 || gridX >= LEVEL_COLS || gridY < 0/* || gridY >= LEVEL_ROWS*/) return 1;

  return tiles[gridY][gridX];
}

int Level::getLevelIndex() {
  return levelIndex;
}
