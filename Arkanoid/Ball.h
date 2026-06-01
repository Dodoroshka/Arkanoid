#pragma once
#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(float startX, float startY);
    Ball(const Ball& other);
    void move(float speedFactor = 1.0f);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void bounceX();
    void bounceY();
    void respawn(float x, float y);
    void setPosition(float newX, float newY);
    float getX() const { return x; }
    float getY() const { return y; }
    float getVelocityX() const { return dx; }
    float getVelocityY() const { return dy; }
    void setVelocity(float vx, float vy) { dx = vx; dy = vy; }

private:
    float x, y;
    float dx, dy;
    float radius;
    float startX, startY;
    static sf::Texture texture;
    sf::Sprite sprite;
};