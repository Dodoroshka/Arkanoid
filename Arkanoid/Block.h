#pragma once
#include <SFML/Graphics.hpp>
#include <array>

class Block {
public:
    Block(float x, float y, int hits = 1);
    Block(const Block& other);
    void draw(sf::RenderWindow& window);
    void hit();
    bool isAlive() const { return hp > 0; }
    bool isIndestructible() const { return indestructible; }
    sf::FloatRect getBounds() const;
    float getX() const { return x; }
    float getY() const { return y; }

private:
    float x, y;
    float width, height;
    int hp;
    bool indestructible;
    static std::array<sf::Texture, 5> textures;
    static bool texturesLoaded;
    sf::Sprite sprite;
    void updateSprite();
};