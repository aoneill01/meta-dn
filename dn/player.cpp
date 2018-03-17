#include <Gamebuino-Meta.h>
#include "player.h"
#include "sprites.h"

#define PRECISION 6
#define GRAVITY 0x5
#define ALMOST_ONE 0b111111

#define velocityInPixelsPerFrame(val) (val << PRECISION >> 1)

void Player::resetPosition(Level &l) {
  x = (20 * 4) << PRECISION;
  y = 0;

  if (l.getLevelIndex() == 0) {
    x = 0;
    y = (18 * 4) << PRECISION;
  }
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
  internalUpdate(level, true);
  internalUpdate(level, false);

  // TODO Replace magic 32 and move this outside of Player
  if (getY() > 32 * 4) {
    level.load(level.getLevelIndex() + 1);
    resetPosition(level);
    gb.display.drawImage(0, 0, *level.background);
  }
}

void Player::internalUpdate(Level &level, bool firstUpdate) {
  
  playerSprite.setFrame(0);

  if (wallJumpDelay) {
    wallJumpDelay--;
  }
  else {
    velX = 0;
  }

  
  if (gb.buttons.repeat(Button::right, 0) && !wallJumpDelay) {
    facingLeft = false;
    velX = velocityInPixelsPerFrame(1);
    playerSprite.setFrame(3 + ((gb.frameCount >> 1) % 4));
  }
  if (gb.buttons.repeat(Button::left, 0) && !wallJumpDelay) {
    facingLeft = true;
    velX = velocityInPixelsPerFrame(-1);
    playerSprite.setFrame(10 + ((gb.frameCount >> 1) % 4));
  }

  if (gb.buttons.pressed(Button::a)) {
    if (touchingGround) {
      velY = velocityInPixelsPerFrame(-3);
    }
    else if (touchingRightWall || touchingLeftWall) {
      velY = velocityInPixelsPerFrame(-3);
      wallJumpDelay = 20;
      if (touchingRightWall) {
        velX = velocityInPixelsPerFrame(-1);
      }
      else {
        velX = velocityInPixelsPerFrame(1);
      }
    }
  }
    
  touchingRightWall = touchingLeftWall = false;
  x += velX;
  if (level.collisionAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight())) {
    wallJumpDelay = 0;
    if (level.lavaAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight())) {
      resetPosition(level);
      return;
    }
    int backOne = velX > 0 ? velocityInPixelsPerFrame(-1) : velocityInPixelsPerFrame(1);
    touchingRightWall = gb.buttons.repeat(Button::right, 0) && firstUpdate && !touchingGround;
    touchingLeftWall = gb.buttons.repeat(Button::left, 0) && firstUpdate && !touchingGround;
    do {
      x += backOne;
    } 
    while(level.collisionAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight()));
  }

  velY += (touchingRightWall || touchingLeftWall) && velY > 0 ? GRAVITY >> 2 : GRAVITY;
  if (velY > 4 * ALMOST_ONE) velY = 4 * ALMOST_ONE;
  if (velY < 4 * -ALMOST_ONE) velY = 4 * -ALMOST_ONE;
  y += velY;
  if (level.collisionAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight())) {
    wallJumpDelay = 0;
    if (level.lavaAt(x >> PRECISION, y >> PRECISION, getWidth(), getHeight())) {
      resetPosition(level);
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
    if (touchingRightWall || touchingLeftWall) {
      if (facingLeft) playerSprite.setFrame(8);
      else playerSprite.setFrame(1);
    }
    else {
      if (facingLeft) playerSprite.setFrame(9);
      else playerSprite.setFrame(2);
    }
  }
}

