#include "Block.h"
#include "utils.h"
#include <iostream>

std::array<sf::Texture, 5> Block::textures;
bool Block::texturesLoaded = false;

Block::Block(float bx, float by, int hits) : x(bx), y(by), width(70.0f), height(25.0f), hp(hits), indestructible(hits == 5) {
    if (!texturesLoaded) {
        std::string base = getExecutableDir();
        std::string paths[5] = {
            base + "assets/blocks/block_red.png",
            base + "assets/blocks/block_orange.png",
            base + "assets/blocks/block_yellow.png",
            base + "assets/blocks/block_green.png",
            base + "assets/blocks/block_blue.png"
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

Block::Block(const Block& other) : x(other.x), y(other.y), width(other.width), height(other.height), hp(other.hp), indestructible(other.indestructible) {
    updateSprite();
}

void Block::updateSprite() {
    int idx = hp - 1;
    if (idx < 0) idx = 0;
    if (idx > 4) idx = 4;
    sprite.setTexture(textures[idx], true);
    sprite.setPosition(x, y);
    sprite.setScale(width / textures[idx].getSize().x, height / textures[idx].getSize().y);
}

void Block::draw(sf::RenderWindow& window) {
    if (hp > 0) window.draw(sprite);
}

void Block::hit() {
    if (hp <= 0) return;
    if (indestructible) return; // неразрушимый — не получает урон
    hp--;
    if (hp > 0) updateSprite();
}

sf::FloatRect Block::getBounds() const {
    return sf::FloatRect(x, y, width, height);
}