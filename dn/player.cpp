#include <Gamebuino-Meta.h>
#include "player.h"
#include "sprites.h"

#define PRECISION 6
#define GRAVITY 0x5
#define ALMOST_ONE 0b111111

#define velocityInPixelsPerFrame(val) (val << PRECISION >> 1)

void Player::resetPosition() {
  x = 80 << PRECISION;
  y = 0;
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

  if (gb.buttons.pressed(Button::a) && (touchingGround || touchingWall)) {
    velY = velocityInPixelsPerFrame(-3);
  }

  touchingWall = false;
  x += velX;
  if (level.collisionAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight())) {
    if (level.lavaAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight())) {
      resetPosition();
      return;
    }
    int backOne = velX > 0 ? velocityInPixelsPerFrame(-1) : velocityInPixelsPerFrame(1);
    touchingWall = gb.buttons.repeat(Button::right, 0) || gb.buttons.repeat(Button::left, 0);
    do {
      x += backOne;
    } 
    while(level.collisionAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight()));
  }

  velY += touchingWall && velY > 0 ? GRAVITY >> 2 : GRAVITY;
  if (velY > 4 * ALMOST_ONE) velY = 4 * ALMOST_ONE;
  if (velY < 4 * -ALMOST_ONE) velY = 4 * -ALMOST_ONE;
  y += velY;
  if (level.collisionAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight())) {
    if (level.lavaAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight())) {
      resetPosition();
      return;
    }
    int backOne = velY > 0 ? velocityInPixelsPerFrame(-1) : velocityInPixelsPerFrame(1);
    velY = 0;
    y = y | ALMOST_ONE;
    do {
      y += backOne;
    }
    while(level.collisionAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight()));
  }

  touchingGround = level.collisionAt(x >> PRECISION, ((y  + 1) >> PRECISION), getWidth(), getHeight());
  if (touchingGround) {
    velY = 0;
  }
  else {
    if (touchingWall) {
      if (facingLeft) playerSprite.setFrame(8);
      else playerSprite.setFrame(1);
    }
    else {
      if (facingLeft) playerSprite.setFrame(9);
      else playerSprite.setFrame(2);
    }
  }
}

