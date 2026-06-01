#pragma once
#include <SFML/Graphics.hpp>
#include <array>

enum class PowerUpType {
    EXTEND_PADDLE,
    MULTI_BALL,
    SHORT_PADDLE,
    FAST_BALL,
    SLOW_PADDLE
};

class PowerUp {
public:
    PowerUp(float x, float y, PowerUpType type);
    PowerUp(const PowerUp& other);
    void update();
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    PowerUpType getType() const { return type; }
    float getY() const { return y; }

private:
    float x, y;
    PowerUpType type;
    float speed;
    static std::array<sf::Texture, 5> textures;
    static bool texturesLoaded;
    sf::Sprite sprite;
    void updateSprite();
};