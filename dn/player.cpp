#include <Gamebuino-Meta.h>
#include "player.h"
#include "sprites.h"

#define PRECISION 6
#define GRAVITY 0x5

#define velocityInPixelsPerFrame(val) (val << PRECISION >> 1)

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

void Player::update(Level &level) {
  internalUpdate(level);
  internalUpdate(level);
}

void Player::internalUpdate(Level &level) {
  velX = 0;
  playerSprite.setFrame(0);
  
  if (gb.buttons.repeat(Button::right, 0)) {
    facingLeft = false;
    velX = velocityInPixelsPerFrame(1);
    playerSprite.setFrame(3 + ((gb.frameCount >> 1) % 4));
  }
  if (gb.buttons.repeat(Button::left, 0)) {
    facingLeft = true;
    velX = velocityInPixelsPerFrame(-1);
    playerSprite.setFrame(10 + ((gb.frameCount >> 1) % 4));
  }

  if (gb.buttons.pressed(Button::a) && onGround) {
    velY = velocityInPixelsPerFrame(-3);
  }

  x += velX;
  if (level.collisionAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight())) {
    int backOne = velX > 0 ? velocityInPixelsPerFrame(-1) : velocityInPixelsPerFrame(1);
    do {
      x += backOne;
    } 
    while(level.collisionAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight()));
  }

  velY += GRAVITY;
  y += velY;
  if (level.collisionAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight())) {
    int backOne = velY > 0 ? velocityInPixelsPerFrame(-1) : velocityInPixelsPerFrame(1);
    velY = 0;
    y = (y >> PRECISION) << PRECISION;
    while(level.collisionAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight())) {
      y += backOne;
    }
  }

  onGround = level.collisionAt(x >> PRECISION, (y >> PRECISION) + 1, getWidth(), getHeight());
  if (onGround) {
    velY = 0;
  }
  else {
    if (facingLeft) playerSprite.setFrame(9);
    else playerSprite.setFrame(2);
  }
}

