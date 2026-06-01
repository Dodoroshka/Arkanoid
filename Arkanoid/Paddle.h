#pragma once
#include <SFML/Graphics.hpp>

class Paddle {
public:
    Paddle();
    void moveLeft(float speed);
    void moveRight(float speed);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void reset();
    void widen();
    void narrow();
    void setWidth(float w);
    void resetWidth();
    void setSpeed(float s);
    float getSpeed() const;
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }

    void setShortPaddle(bool active, float duration);
    void setSlowPaddle(bool active, float duration);
    void updateEffects(float dt);
    bool isShort() const { return shortActive; }
    bool isSlow() const { return slowActive; }

private:
    float x, y;
    float width, height;
    float defaultWidth;
    float speed;
    float defaultSpeed;
    bool shortActive;
    float shortTimer;
    bool slowActive;
    float slowTimer;

    sf::Texture textureNormal;
    sf::Texture textureLong;
    sf::Texture textureShort;
    sf::Sprite sprite;
    void updateSprite();
};