#include "PowerUp.h"
#include "utils.h"
#include <iostream>

std::array<sf::Texture, 5> PowerUp::textures;
bool PowerUp::texturesLoaded = false;

PowerUp::PowerUp(float px, float py, PowerUpType t) : x(px), y(py), type(t), speed(2.0f) {
    if (!texturesLoaded) {
        std::string base = getExecutableDir();
        // Порядок СТРОГО совпадает с enum PowerUpType:
        // 0 = EXTEND_PADDLE
        // 1 = MULTI_BALL
        // 2 = SHORT_PADDLE
        // 3 = FAST_BALL
        // 4 = SLOW_PADDLE
        std::string paths[5] = {
            base + "assets/powerups/powerup_extend.png",    // EXTEND_PADDLE
            base + "assets/powerups/powerup_multiball.png", // MULTI_BALL
            base + "assets/powerups/powerup_short.png",     // SHORT_PADDLE
            base + "assets/powerups/powerup_fastball.png",  // FAST_BALL
            base + "assets/powerups/powerup_slowpaddle.png" // SLOW_PADDLE
        };
        for (int i = 0; i < 5; ++i) {
            if (!textures[i].loadFromFile(paths[i]))
                std::cerr << "ERROR: Failed to load " << paths[i] << std::endl;
            textures[i].setSmooth(true);
        }
        texturesLoaded = true;
    }
    updateSprite();
}

PowerUp::PowerUp(const PowerUp& other) : x(other.x), y(other.y), type(other.type), speed(other.speed) {
    updateSprite();
}

void PowerUp::updateSprite() {
    int idx = static_cast<int>(type);
    if (idx < 0) idx = 0;
    if (idx > 4) idx = 4;
    sprite.setTexture(textures[idx], true);
    sprite.setPosition(x, y);
    if (textures[idx].getSize().x > 0 && textures[idx].getSize().y > 0)
        sprite.setScale(20.0f / textures[idx].getSize().x, 20.0f / textures[idx].getSize().y);
}

void PowerUp::update() {
    y += speed;
    sprite.setPosition(x, y);
}

void PowerUp::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect PowerUp::getBounds() const {
    return sprite.getGlobalBounds();
}