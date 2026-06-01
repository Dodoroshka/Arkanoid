#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Ball.h"
#include "Paddle.h"
#include "Block.h"
#include "Level.h"
#include "PowerUp.h"

enum class GameState {
    MAIN_MENU,
    PLAYING,
    MULTIPLAYER_MENU,
    MULTIPLAYER_PLAYING,
    SECRET_UNLOCKING,
    SECRET_LEVEL,
    PAUSE,
    GAME_OVER,
    YOU_WIN
};

struct MatrixDrop {
    float x, y, speed;
    char ch; // '0' или '1'
    float brightness; // 0.0 - 1.0
};

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void handleCollisions();
    void spawnPowerUp(float x, float y, PowerUpType type);
    void applyPowerUp(PowerUpType type, bool forLeftPaddle);
    int countAliveBlocks() const;
    void checkSecretUnlock();
    void loadSecretLevel();
    void loadNormalLevel(int level);
    void loadMultiplayerLevel(int level);
    void resetDebuffs();
    void resetGame();
    void restartSingleplayer();
    void restartMultiplayer();
    void toggleFullscreen();

    void initMatrixRain();
    void updateMatrixRain(float dt);
    void drawMatrixRain();

    sf::RenderWindow window;
    GameState state;
    GameState previousState;
    bool isRunning;
    int score;
    int lives;
    int currentLevel;
    int multiplayerLevel;

    Paddle paddle;
    Paddle paddleLeft;
    Paddle paddleRight;

    std::vector<Ball> balls;
    std::vector<Block> blocks;
    std::vector<PowerUp> powerUps;

    Level levelLoader;

    bool fastBallMode;
    float fastBallTimer;

    int secretHitCount;
    float secretTimer;

    int menuIndex;
    int pauseMenuIndex;
    sf::Font font;
    sf::Sprite logoSprite;
    sf::Texture logoTexture;
    sf::Sprite backgroundMenu;
    sf::Texture backgroundMenuTex;
    sf::Sprite backgroundGame;
    sf::Texture backgroundGameTex;

    struct MenuButton {
        sf::Texture normal, hover;
        sf::Sprite sprite;
    };
    std::vector<MenuButton> menuButtons;
    std::vector<MenuButton> pauseButtons;

    bool isFullscreen;
    sf::VideoMode desktopMode;

    // Матричный дождь
    std::vector<MatrixDrop> matrixDrops;
    float matrixTimer;

    void setupMenu();
    void drawMenu();
    void drawGameOver();
    void drawYouWin();
    void drawMultiplayerMenu();
    void drawPauseMenu();
};