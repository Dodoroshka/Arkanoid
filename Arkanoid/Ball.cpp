#include "Ball.h"
#include "utils.h"
#include <iostream>

sf::Texture Ball::texture;

Ball::Ball(float startX, float startY) : startX(startX), startY(startY) {
    x = startX;
    y = startY;
    radius = 8.0f;
    dx = 3.0f;
    dy = -3.0f;

    if (texture.getSize().x == 0) {
        std::string base = getExecutableDir();
        std::string ballPath = base + "assets/balls/ball.png";
        if (!texture.loadFromFile(ballPath))
            std::cerr << "ERROR: Failed to load " << ballPath << std::endl;
        texture.setSmooth(true);
    }

    sprite.setTexture(texture);
    sprite.setOrigin(8, 8);
    sprite.setPosition(x, y);
}

Ball::Ball(const Ball& other) : x(other.x), y(other.y), dx(other.dx), dy(other.dy),
radius(other.radius), startX(other.startX), startY(other.startY) {
    sprite.setTexture(texture);
    sprite.setOrigin(8, 8);
    sprite.setPosition(x, y);
}

void Ball::move(float speedFactor) {
    x += dx * speedFactor;
    y += dy * speedFactor;
    sprite.setPosition(x, y);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Ball::getBounds() const {
    return sf::FloatRect(x - radius, y - radius, radius * 2, radius * 2);
}

void Ball::bounceX() { dx = -dx; }
void Ball::bounceY() { dy = -dy; }

void Ball::respawn(float sx, float sy) {
    x = sx; y = sy;
    dx = 3.0f; dy = -3.0f;
    sprite.setPosition(x, y);
}

void Ball::setPosition(float newX, float newY) {
    x = newX; y = newY;
    sprite.setPosition(x, y);
}