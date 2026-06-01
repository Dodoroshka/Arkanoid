#include "Paddle.h"
#include "utils.h"
#include <iostream>

Paddle::Paddle() : x(350), y(555), width(100), height(18),
defaultWidth(100), speed(8.0f), defaultSpeed(8.0f),
shortActive(false), shortTimer(0), slowActive(false), slowTimer(0)
{
    std::string base = getExecutableDir();

    std::string normalPath = base + "assets/paddles/paddle_normal.png";
    if (!textureNormal.loadFromFile(normalPath))
        std::cerr << "ERROR: Failed to load " << normalPath << std::endl;

    std::string longPath = base + "assets/paddles/paddle_long.png";
    if (!textureLong.loadFromFile(longPath))
        std::cerr << "ERROR: Failed to load " << longPath << std::endl;

    std::string shortPath = base + "assets/paddles/paddle_short.png";
    if (!textureShort.loadFromFile(shortPath))
        std::cerr << "ERROR: Failed to load " << shortPath << std::endl;

    textureNormal.setSmooth(true);
    textureLong.setSmooth(true);
    textureShort.setSmooth(true);
    updateSprite();
}

void Paddle::updateSprite() {
    if (shortActive)
        sprite.setTexture(textureShort, true);
    else if (width > defaultWidth)
        sprite.setTexture(textureLong, true);
    else
        sprite.setTexture(textureNormal, true);

    sprite.setPosition(x, y);
    const sf::Texture* tex = sprite.getTexture();
    if (tex && tex->getSize().x > 0 && tex->getSize().y > 0) {
        sprite.setScale(width / tex->getSize().x, height / tex->getSize().y);
    }
}

void Paddle::moveLeft(float spd) {
    x -= spd;
    if (x < 10) x = 10;
    updateSprite();
}

void Paddle::moveRight(float spd) {
    x += spd;
    if (x + width > 790) x = 790 - width;
    updateSprite();
}

void Paddle::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Paddle::getBounds() const {
    return sf::FloatRect(x, y, width, height);
}

void Paddle::reset() {
    x = 350;
    width = defaultWidth;
    speed = defaultSpeed;
    shortActive = false;
    slowActive = false;
    shortTimer = 0;
    slowTimer = 0;
    updateSprite();
}

void Paddle::widen() {
    if (width < 200) {
        x -= 20;
        width += 40;
        if (x < 10) x = 10;
        if (x + width > 790) x = 790 - width;
        updateSprite();
    }
}

void Paddle::narrow() {
    if (width > 40) {
        x += 20;
        width -= 40;
        if (x < 10) x = 10;
        if (x + width > 790) x = 790 - width;
        updateSprite();
    }
}

void Paddle::setWidth(float w) {
    if (w >= 30 && w <= 250) {
        x += (width - w) / 2;
        width = w;
        if (x < 10) x = 10;
        if (x + width > 790) x = 790 - width;
        updateSprite();
    }
}

void Paddle::resetWidth() {
    width = defaultWidth;
    if (x < 10) x = 10;
    if (x + width > 790) x = 790 - width;
    updateSprite();
}

void Paddle::setSpeed(float s) {
    speed = s;
}

float Paddle::getSpeed() const {
    return speed;
}

void Paddle::setShortPaddle(bool active, float duration) {
    shortActive = active;
    if (active) {
        shortTimer = duration;
        setWidth(60);
    }
    else {
        resetWidth();
    }
    updateSprite();
}

void Paddle::setSlowPaddle(bool active, float duration) {
    slowActive = active;
    if (active) {
        slowTimer = duration;
        setSpeed(defaultSpeed / 2.0f);
    }
    else {
        setSpeed(defaultSpeed);
    }
}

void Paddle::updateEffects(float dt) {
    if (shortActive) {
        shortTimer -= dt;
        if (shortTimer <= 0) {
            shortActive = false;
            resetWidth();
        }
    }
    if (slowActive) {
        slowTimer -= dt;
        if (slowTimer <= 0) {
            slowActive = false;
            setSpeed(defaultSpeed);
        }
    }
    updateSprite();
}