#include <Gamebuino-Meta.h>
#include "player.h"
#include "sprites.h"

#define PRECISION 4
#define GRAVITY 0x2

#define TMP_GROUND 80

void Player::move(int x, int y) {
  this->x = x;
  this->y = y;
}

int Player::getX() {
  return x >> PRECISION;
}

int Player::getY() {
  return y >> PRECISION;
}
  
void Player::draw() {
  gb.display.drawImage(getX(), getY(), playerSprite);
}

void Player::update() {
  playerSprite.setFrame(0);
  
  if (gb.buttons.repeat(Button::right, 0)) {
    x += 1 << PRECISION;
    playerSprite.setFrame(3 + ((gb.frameCount >> 1) % 4));
  }
  if (gb.buttons.repeat(Button::left, 0)) {
    x -= 1 << PRECISION;
    playerSprite.setFrame(10 + ((gb.frameCount >> 1) % 4));
  }

  if (gb.buttons.repeat(Button::a, 0) && getY() == TMP_GROUND) {
    x -= 1 << PRECISION;
    velY = -2 << PRECISION;
  }

  velY += GRAVITY;
  y += velY;
    
  if (getY() < TMP_GROUND) {
    playerSprite.setFrame(2);  
  }
  
  if (getY() >= TMP_GROUND) {
    velY = 0;
    y = TMP_GROUND << PRECISION;
  }
}

