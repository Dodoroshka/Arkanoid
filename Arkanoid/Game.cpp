#include "Game.h"
#include "utils.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>

Game::Game() : state(GameState::MAIN_MENU), previousState(GameState::MAIN_MENU), isRunning(true), score(0), lives(3), currentLevel(1), multiplayerLevel(0),
fastBallMode(false), fastBallTimer(0),
secretHitCount(0), secretTimer(0), menuIndex(0), pauseMenuIndex(0), isFullscreen(false), matrixTimer(0)
{
    srand(static_cast<unsigned>(time(nullptr)));
    desktopMode = sf::VideoMode::getDesktopMode();
    window.create(sf::VideoMode(800, 600), "Arkanoid", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    setupMenu();
}

void Game::toggleFullscreen() {
    isFullscreen = !isFullscreen;
    window.close();
    if (isFullscreen) {
        window.create(desktopMode, "Arkanoid", sf::Style::Fullscreen);
        sf::View view(sf::FloatRect(0, 0, 800, 600));
        float windowAspect = static_cast<float>(desktopMode.width) / desktopMode.height;
        float gameAspect = 800.0f / 600.0f;
        if (windowAspect > gameAspect) {
            float viewportWidth = gameAspect / windowAspect;
            view.setViewport(sf::FloatRect((1 - viewportWidth) / 2, 0, viewportWidth, 1));
        }
        else {
            float viewportHeight = windowAspect / gameAspect;
            view.setViewport(sf::FloatRect(0, (1 - viewportHeight) / 2, 1, viewportHeight));
        }
        window.setView(view);
    }
    else {
        window.create(sf::VideoMode(800, 600), "Arkanoid", sf::Style::Titlebar | sf::Style::Close);
        window.setView(sf::View(sf::FloatRect(0, 0, 800, 600)));
    }
    window.setFramerateLimit(60);
}

void Game::setupMenu() {
    std::string base = getExecutableDir();

    if (!font.loadFromFile(base + "assets/fonts/Curtsweeper-Regular.ttf"))
        std::cerr << "Failed to load font" << std::endl;
    font.setSmooth(true);

    if (!backgroundMenuTex.loadFromFile(base + "assets/backgrounds/background_menu.png"))
        std::cerr << "Failed to load background menu" << std::endl;
    if (!backgroundGameTex.loadFromFile(base + "assets/backgrounds/background_game.png"))
        std::cerr << "Failed to load background game" << std::endl;
    backgroundMenu.setTexture(backgroundMenuTex);
    backgroundGame.setTexture(backgroundGameTex);
    backgroundMenu.setScale(800.0f / backgroundMenuTex.getSize().x, 600.0f / backgroundMenuTex.getSize().y);
    backgroundGame.setScale(800.0f / backgroundGameTex.getSize().x, 600.0f / backgroundGameTex.getSize().y);

    if (!logoTexture.loadFromFile(base + "assets/logo/logo.png"))
        std::cerr << "Failed to load logo" << std::endl;
    logoSprite.setTexture(logoTexture);
    logoSprite.setOrigin(logoTexture.getSize().x / 2, logoTexture.getSize().y / 2);
    logoSprite.setPosition(400, 120);
    float logoScale = std::min(400.0f / logoTexture.getSize().x, 150.0f / logoTexture.getSize().y);
    logoSprite.setScale(logoScale, logoScale);

    std::vector<std::string> menuItems = { "start", "multiplayer", "exit" };
    for (const auto& name : menuItems) {
        MenuButton btn;
        if (!btn.normal.loadFromFile(base + "assets/buttons/" + name + "_normal.png"))
            std::cerr << "Failed to load button " << name << "_normal.png" << std::endl;
        if (!btn.hover.loadFromFile(base + "assets/buttons/" + name + "_hover.png"))
            std::cerr << "Failed to load button " << name << "_hover.png" << std::endl;
        btn.sprite.setTexture(btn.normal);
        menuButtons.push_back(btn);
    }
    for (int i = 0; i < (int)menuButtons.size(); ++i) {
        float x = 400 - menuButtons[i].sprite.getGlobalBounds().width / 2;
        float y = 250 + i * 70;
        menuButtons[i].sprite.setPosition(x, y);
    }

    std::vector<std::string> pauseItems = { "resume", "restart", "exit_menu" };
    for (const auto& name : pauseItems) {
        MenuButton btn;
        if (!btn.normal.loadFromFile(base + "assets/buttons/" + name + "_normal.png"))
            std::cerr << "Failed to load pause button " << name << "_normal.png" << std::endl;
        if (!btn.hover.loadFromFile(base + "assets/buttons/" + name + "_hover.png"))
            std::cerr << "Failed to load pause button " << name << "_hover.png" << std::endl;
        btn.sprite.setTexture(btn.normal);
        pauseButtons.push_back(btn);
    }
    for (int i = 0; i < (int)pauseButtons.size(); ++i) {
        float x = 400 - pauseButtons[i].sprite.getGlobalBounds().width / 2;
        float y = 300 + i * 60;
        pauseButtons[i].sprite.setPosition(x, y);
    }
}

void Game::drawMenu() {
    window.clear();
    window.draw(backgroundMenu);
    window.draw(logoSprite);
    for (int i = 0; i < (int)menuButtons.size(); ++i) {
        if (i == menuIndex)
            menuButtons[i].sprite.setTexture(menuButtons[i].hover);
        else
            menuButtons[i].sprite.setTexture(menuButtons[i].normal);
        window.draw(menuButtons[i].sprite);
    }
    window.display();
}

void Game::drawMultiplayerMenu() {
    window.clear();
    window.draw(backgroundMenu);
    window.draw(logoSprite);

    sf::Text title;
    title.setFont(font);
    title.setString("MULTIPLAYER MODE");
    title.setCharacterSize(28);
    title.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(400 - titleBounds.width / 2, 250);
    window.draw(title);

    sf::Text prompt;
    prompt.setFont(font);
    prompt.setString("Press ENTER to start\n(3 lives, 3 levels)");
    prompt.setCharacterSize(18);
    prompt.setFillColor(sf::Color::White);
    sf::FloatRect promptBounds = prompt.getLocalBounds();
    prompt.setPosition(400 - promptBounds.width / 2, 320);
    window.draw(prompt);

    sf::Text back;
    back.setFont(font);
    back.setString("Press ESC to return");
    back.setCharacterSize(18);
    back.setFillColor(sf::Color::White);
    sf::FloatRect backBounds = back.getLocalBounds();
    back.setPosition(400 - backBounds.width / 2, 390);
    window.draw(back);

    window.display();
}

void Game::drawGameOver() {
    window.clear();
    window.draw(backgroundGame);
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Red);
    text.setString("GAME OVER");
    sf::FloatRect tb = text.getLocalBounds();
    text.setPosition(400 - tb.width / 2, 200);
    window.draw(text);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(25);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("Score: " + std::to_string(score));
    sf::FloatRect sb = scoreText.getLocalBounds();
    scoreText.setPosition(400 - sb.width / 2, 280);
    window.draw(scoreText);

    sf::Text restartText;
    restartText.setFont(font);
    restartText.setCharacterSize(18);
    restartText.setFillColor(sf::Color::Green);
    restartText.setString("Press R to RESTART\nPress ESC to EXIT");
    sf::FloatRect rb = restartText.getLocalBounds();
    restartText.setPosition(400 - rb.width / 2, 370);
    window.draw(restartText);
    window.display();
}

void Game::initMatrixRain() {
    matrixDrops.clear();
    matrixTimer = 0;
    // Создаём ~120 капель по всей ширине экрана
    for (int i = 0; i < 120; ++i) {
        MatrixDrop drop;
        drop.x = static_cast<float>(rand() % 790);
        drop.y = static_cast<float>(rand() % 600) - 600.0f; // стартуют выше экрана
        drop.speed = 80.0f + (rand() % 120);
        drop.ch = (rand() % 2 == 0) ? '0' : '1';
        drop.brightness = 0.4f + (rand() % 60) / 100.0f;
        matrixDrops.push_back(drop);
    }
}

void Game::updateMatrixRain(float dt) {
    matrixTimer += dt;
    for (auto& drop : matrixDrops) {
        drop.y += drop.speed * dt;
        // Случайно меняем символ
        if (rand() % 20 == 0) drop.ch = (rand() % 2 == 0) ? '0' : '1';
        // Когда улетает вниз — сброс наверх
        if (drop.y > 610) {
            drop.y = static_cast<float>(rand() % 100) - 100.0f;
            drop.x = static_cast<float>(rand() % 790);
            drop.speed = 80.0f + (rand() % 120);
            drop.brightness = 0.4f + (rand() % 60) / 100.0f;
        }
    }
}

void Game::drawMatrixRain() {
    sf::Text ch;
    ch.setFont(font);
    ch.setCharacterSize(16);
    for (auto& drop : matrixDrops) {
        int g = static_cast<int>(drop.brightness * 255);
        ch.setFillColor(sf::Color(0, g, 0));
        ch.setString(drop.ch);
        ch.setPosition(drop.x, drop.y);
        window.draw(ch);
    }
}

void Game::drawYouWin() {
    window.clear();
    window.draw(backgroundGame);

    // Матричный дождь
    drawMatrixRain();

    // Полупрозрачный оверлей поверх дождя
    sf::RectangleShape overlay(sf::Vector2f(800, 600));
    overlay.setFillColor(sf::Color(0, 0, 0, 140));
    window.draw(overlay);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Yellow);
    text.setString("YOU WIN!");
    sf::FloatRect tb = text.getLocalBounds();
    text.setPosition(400 - tb.width / 2, 180);
    window.draw(text);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(25);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("Final score: " + std::to_string(score));
    sf::FloatRect sb = scoreText.getLocalBounds();
    scoreText.setPosition(400 - sb.width / 2, 260);
    window.draw(scoreText);

    sf::Text restartText;
    restartText.setFont(font);
    restartText.setCharacterSize(18);
    restartText.setFillColor(sf::Color(0, 220, 0));
    restartText.setString("Press R to RESTART\nPress ESC to EXIT");
    sf::FloatRect rb = restartText.getLocalBounds();
    restartText.setPosition(400 - rb.width / 2, 350);
    window.draw(restartText);
    window.display();
}

void Game::drawPauseMenu() {
    window.clear();
    window.draw(backgroundGame);
    sf::RectangleShape overlay(sf::Vector2f(800, 600));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::Text pauseTitle;
    pauseTitle.setFont(font);
    pauseTitle.setString("PAUSED");
    pauseTitle.setCharacterSize(50);
    pauseTitle.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleBounds = pauseTitle.getLocalBounds();
    pauseTitle.setPosition(400 - titleBounds.width / 2, 150);
    window.draw(pauseTitle);

    for (int i = 0; i < (int)pauseButtons.size(); ++i) {
        if (i == pauseMenuIndex)
            pauseButtons[i].sprite.setTexture(pauseButtons[i].hover);
        else
            pauseButtons[i].sprite.setTexture(pauseButtons[i].normal);
        window.draw(pauseButtons[i].sprite);
    }
    window.display();
}

void Game::run() {
    while (window.isOpen() && isRunning) {
        processEvents();
        update();
        render();
        sf::sleep(sf::milliseconds(8));
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::F11) toggleFullscreen();
            if (state == GameState::GAME_OVER || state == GameState::YOU_WIN) {
                if (event.key.code == sf::Keyboard::R) {
                    if (previousState == GameState::MULTIPLAYER_PLAYING) restartMultiplayer();
                    else restartSingleplayer();
                }
                else if (event.key.code == sf::Keyboard::Escape) state = GameState::MAIN_MENU;
                continue;
            }
            if (state == GameState::MAIN_MENU) {
                if (event.key.code == sf::Keyboard::Up) menuIndex = (menuIndex - 1 + menuButtons.size()) % menuButtons.size();
                else if (event.key.code == sf::Keyboard::Down) menuIndex = (menuIndex + 1) % menuButtons.size();
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (menuIndex == 0) { state = GameState::PLAYING; resetGame(); }
                    else if (menuIndex == 1) state = GameState::MULTIPLAYER_MENU;
                    else if (menuIndex == 2) window.close();
                }
            }
            else if (state == GameState::MULTIPLAYER_MENU) {
                if (event.key.code == sf::Keyboard::Enter) {
                    state = GameState::MULTIPLAYER_PLAYING;
                    multiplayerLevel = 1; score = 0; lives = 3;
                    loadMultiplayerLevel(1);
                }
                else if (event.key.code == sf::Keyboard::Escape) state = GameState::MAIN_MENU;
            }
            else if (state == GameState::PLAYING || state == GameState::MULTIPLAYER_PLAYING || state == GameState::SECRET_LEVEL) {
                if (event.key.code == sf::Keyboard::Escape) { previousState = state; state = GameState::PAUSE; pauseMenuIndex = 0; }
            }
            else if (state == GameState::PAUSE) {
                if (event.key.code == sf::Keyboard::Up) pauseMenuIndex = (pauseMenuIndex - 1 + pauseButtons.size()) % pauseButtons.size();
                else if (event.key.code == sf::Keyboard::Down) pauseMenuIndex = (pauseMenuIndex + 1) % pauseButtons.size();
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (pauseMenuIndex == 0) state = previousState;
                    else if (pauseMenuIndex == 1) {
                        if (previousState == GameState::MULTIPLAYER_PLAYING) restartMultiplayer();
                        else restartSingleplayer();
                    }
                    else if (pauseMenuIndex == 2) state = GameState::MAIN_MENU;
                }
                else if (event.key.code == sf::Keyboard::Escape) state = previousState;
            }
        }
    }

    if (state == GameState::PLAYING || state == GameState::SECRET_LEVEL || state == GameState::SECRET_UNLOCKING) {
        float move = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) move = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) move = 1;
        float currentSpeed = paddle.isSlow() ? paddle.getSpeed() : 8.0f;
        if (move < 0) paddle.moveLeft(currentSpeed);
        if (move > 0) paddle.moveRight(currentSpeed);
    }
    else if (state == GameState::MULTIPLAYER_PLAYING) {
        float move1 = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) move1 = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) move1 = 1;
        float speed1 = paddleLeft.isSlow() ? paddleLeft.getSpeed() : 8.0f;
        if (move1 < 0) paddleLeft.moveLeft(speed1);
        if (move1 > 0) paddleLeft.moveRight(speed1);

        float move2 = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) move2 = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) move2 = 1;
        float speed2 = paddleRight.isSlow() ? paddleRight.getSpeed() : 8.0f;
        if (move2 < 0) paddleRight.moveLeft(speed2);
        if (move2 > 0) paddleRight.moveRight(speed2);

        if (paddleLeft.getX() + paddleLeft.getWidth() > 390) paddleLeft.setPosition(390 - paddleLeft.getWidth(), paddleLeft.getY());
        if (paddleRight.getX() < 410) paddleRight.setPosition(410, paddleRight.getY());
    }
}

void Game::update() {
    float dt = 1.0f / 60.0f;

    if (state == GameState::YOU_WIN) {
        updateMatrixRain(dt);
        return;
    }

    if (state == GameState::SECRET_UNLOCKING) {
        paddle.updateEffects(dt);
        secretTimer -= dt;
        if (secretTimer <= 0) {
            // время вышло — показываем победный экран с дождём
            previousState = GameState::PLAYING;
            state = GameState::YOU_WIN;
            initMatrixRain();
            return;
        }
        checkSecretUnlock();
        return;
    }

    if (state == GameState::PLAYING || state == GameState::SECRET_LEVEL || state == GameState::MULTIPLAYER_PLAYING) {
        if (state == GameState::MULTIPLAYER_PLAYING) {
            paddleLeft.updateEffects(dt);
            paddleRight.updateEffects(dt);
        }
        else {
            paddle.updateEffects(dt);
        }
        if (fastBallMode) {
            fastBallTimer -= dt;
            if (fastBallTimer <= 0) fastBallMode = false;
        }
        float ballSpeedFactor = fastBallMode ? 1.4f : 1.0f;
        for (auto& ball : balls) ball.move(ballSpeedFactor);
        for (auto& pu : powerUps) pu.update();
        handleCollisions();

        if (countAliveBlocks() == 0) {
            if (state == GameState::SECRET_LEVEL) {
                previousState = state;
                state = GameState::YOU_WIN;
                initMatrixRain();
                return;
            }
            else if (state == GameState::MULTIPLAYER_PLAYING) {
                multiplayerLevel++;
                if (multiplayerLevel > 3) {
                    previousState = state;
                    state = GameState::YOU_WIN;
                    initMatrixRain();
                }
                else {
                    loadMultiplayerLevel(multiplayerLevel);
                }
            }
            else if (state == GameState::PLAYING) {
                if (currentLevel >= 5) {
                    state = GameState::SECRET_UNLOCKING;
                    secretTimer = 20.0f;
                    secretHitCount = 0;
                    currentLevel = 5;
                }
                else {
                    currentLevel++;
                    loadNormalLevel(currentLevel);
                }
            }
        }
    }
}

void Game::checkSecretUnlock() {
    static bool wasHit = false;
    bool atRightWall = (paddle.getX() + paddle.getWidth() >= 790);
    bool movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    if (atRightWall && movingRight && !wasHit) {
        secretHitCount++;
        wasHit = true;
    }
    if (!atRightWall || !movingRight) wasHit = false;
    if (secretHitCount >= 3 && state == GameState::SECRET_UNLOCKING) {
        state = GameState::SECRET_LEVEL;
        loadSecretLevel();
    }
}

void Game::loadSecretLevel() {
    blocks.clear();
    float startX = 50.0f, startY = 80.0f;
    float bw = 70.0f, bh = 25.0f, spacing = 5.0f;
    const int grid[6][10] = {
        {0,3,3,0,0,0,4,4,4,4},
        {3,0,0,3,0,0,0,0,0,4},
        {3,0,0,0,0,0,0,0,4,0},
        {3,3,3,0,0,0,0,4,0,0},
        {3,0,0,3,0,0,4,0,0,0},
        {0,3,3,0,0,0,4,0,0,0}
    };
    for (int row = 0; row < 6; ++row)
        for (int col = 0; col < 10; ++col)
            if (grid[row][col] > 0)
                blocks.push_back(Block(startX + col * (bw + spacing), startY + row * (bh + spacing), grid[row][col]));

    balls.clear();
    balls.reserve(10);
    balls.push_back(Ball(400, 500));
    paddle.reset();
    resetDebuffs();
    powerUps.clear();
    powerUps.reserve(50);
    score += 500;
    currentLevel = 6;
}

void Game::loadNormalLevel(int level) {
    blocks = levelLoader.loadLevel(level);
    balls.clear();
    balls.reserve(10);
    balls.push_back(Ball(400, 500));
    paddle.reset();
    resetDebuffs();
    powerUps.clear();
    powerUps.reserve(50);
}

void Game::loadMultiplayerLevel(int level) {
    blocks.clear();
    float startX = 50.0f, startY = 80.0f;
    float bw = 70.0f, bh = 25.0f, spacing = 5.0f;

    auto build = [&](const std::vector<std::vector<int>>& grid) {
        for (int row = 0; row < (int)grid.size(); ++row)
            for (int col = 0; col < (int)grid[row].size(); ++col)
                if (grid[row][col] > 0)
                    blocks.push_back(Block(
                        startX + col * (bw + spacing),
                        startY + row * (bh + spacing),
                        grid[row][col]
                    ));
        };

    if (level == 1) {
        build({
            {1,1,1,1,1,1,1,1,1,1},
            {2,2,2,2,2,2,2,2,2,2},
            {3,3,3,0,0,0,0,3,3,3},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,3,0,0,0,0,3,0,0},
            {0,0,3,3,3,3,3,3,0,0}
            });
    }
    else if (level == 2) {
        build({
            {2,2,3,1,1,2,2,3,1,1},
            {2,3,3,2,1,2,3,3,2,1},
            {2,3,0,0,1,2,3,0,0,1},
            {5,4,4,4,5,5,4,4,4,5}
            });
    }
    else if (level == 3) {
        build({
            {0,2,0,2,0,0,2,0,2,0},
            {2,4,2,4,2,2,4,2,4,2},
            {5,2,4,2,0,0,2,4,2,5},
            {0,5,2,0,3,3,0,2,5,0},
            {1,3,1,3,1,1,3,1,3,1}
            });
    }

    balls.clear();
    balls.reserve(10);
    Ball b1(200, 550);
    Ball b2(600, 550);
    b1.setVelocity(2.0f, -2.5f);
    b2.setVelocity(-2.0f, -2.5f);
    balls.push_back(b1);
    balls.push_back(b2);
    paddleLeft.reset();
    paddleRight.reset();
    paddleLeft.setPosition(150, 555);
    paddleRight.setPosition(550, 555);
    resetDebuffs();
    powerUps.clear();
    powerUps.reserve(50);
    multiplayerLevel = level;
}

void Game::resetDebuffs() {
    fastBallMode = false;
    if (state == GameState::MULTIPLAYER_PLAYING) {
        paddleLeft.setShortPaddle(false, 0);
        paddleLeft.setSlowPaddle(false, 0);
        paddleRight.setShortPaddle(false, 0);
        paddleRight.setSlowPaddle(false, 0);
    }
    else {
        paddle.setShortPaddle(false, 0);
        paddle.setSlowPaddle(false, 0);
    }
}

void Game::resetGame() {
    score = 0; lives = 3; currentLevel = 1; multiplayerLevel = 0;
    resetDebuffs();
    loadNormalLevel(1);
    secretHitCount = 0; secretTimer = 0; powerUps.clear();
    state = GameState::PLAYING;
}

void Game::restartSingleplayer() {
    resetGame();
}

void Game::restartMultiplayer() {
    score = 0; lives = 3; multiplayerLevel = 1;
    resetDebuffs();
    loadMultiplayerLevel(1);
    secretHitCount = 0; secretTimer = 0; powerUps.clear();
    state = GameState::MULTIPLAYER_PLAYING;
}

void Game::handleCollisions() {
    for (auto& ball : balls) {
        if (ball.getX() <= 8) { ball.setPosition(8, ball.getY()); ball.bounceX(); }
        if (ball.getX() >= 792) { ball.setPosition(792, ball.getY()); ball.bounceX(); }
        if (ball.getY() <= 8) { ball.setPosition(ball.getX(), 8); ball.bounceY(); }
        if (state == GameState::PLAYING || state == GameState::SECRET_LEVEL) {
            if (ball.getBounds().intersects(paddle.getBounds())) {
                ball.bounceY();
                float hitPos = ball.getX() - paddle.getX();
                float newDx = (hitPos - paddle.getWidth() / 2.0f) / 20.0f;
                if (newDx > -0.5f && newDx < 0.5f) newDx = (newDx > 0) ? 0.8f : -0.8f;
                if (newDx > 3.0f) newDx = 3.0f;
                if (newDx < -3.0f) newDx = -3.0f;
                ball.setVelocity(newDx, -std::abs(ball.getVelocityY()));
                ball.setPosition(ball.getX(), paddle.getY() - 18);
            }
        }
        else if (state == GameState::MULTIPLAYER_PLAYING) {
            if (ball.getBounds().intersects(paddleLeft.getBounds())) {
                ball.bounceY();
                float hitPos = ball.getX() - paddleLeft.getX();
                float newDx = (hitPos - paddleLeft.getWidth() / 2.0f) / 20.0f;
                if (newDx > -0.5f && newDx < 0.5f) newDx = (newDx > 0) ? 0.8f : -0.8f;
                if (newDx > 3.0f) newDx = 3.0f;
                if (newDx < -3.0f) newDx = -3.0f;
                ball.setVelocity(newDx, -std::abs(ball.getVelocityY()));
                ball.setPosition(ball.getX(), paddleLeft.getY() - 18);
            }
            if (ball.getBounds().intersects(paddleRight.getBounds())) {
                ball.bounceY();
                float hitPos = ball.getX() - paddleRight.getX();
                float newDx = (hitPos - paddleRight.getWidth() / 2.0f) / 20.0f;
                if (newDx > -0.5f && newDx < 0.5f) newDx = (newDx > 0) ? 0.8f : -0.8f;
                if (newDx > 3.0f) newDx = 3.0f;
                if (newDx < -3.0f) newDx = -3.0f;
                ball.setVelocity(newDx, -std::abs(ball.getVelocityY()));
                ball.setPosition(ball.getX(), paddleRight.getY() - 18);
            }
        }
    }

    bool anyAlive = false;
    for (auto it = balls.begin(); it != balls.end(); ) {
        if (it->getY() >= 590) it = balls.erase(it);
        else { anyAlive = true; ++it; }
    }
    if (!anyAlive && lives > 0) {
        lives--;
        if (lives > 0) {
            if (state == GameState::MULTIPLAYER_PLAYING) {
                Ball b1(200, 550), b2(600, 550);
                b1.setVelocity(2.0f, -2.5f); b2.setVelocity(-2.0f, -2.5f);
                balls.push_back(b1); balls.push_back(b2);
            }
            else {
                balls.push_back(Ball(400, 500));
            }
        }
        else {
            previousState = state; // сохраняем текущий режим (MULTIPLAYER или PLAYING)
            state = GameState::GAME_OVER;
            return;
        }
    }

    for (auto& ball : balls) {
        for (auto& block : blocks) {
            if (block.isAlive() && ball.getBounds().intersects(block.getBounds())) {
                ball.bounceY();
                if (block.isIndestructible()) {
                    // неразрушимый — отбиваем мяч, но ни очков ни баффов
                    break;
                }
                block.hit();
                score += 10;
                // баффы выпадают только при разрушении блока
                if (!block.isAlive()) {
                    int r = rand() % 100;
                    PowerUpType type;
                    if (r < 40) {
                        int t = rand() % 2;
                        type = (t == 0) ? PowerUpType::EXTEND_PADDLE : PowerUpType::MULTI_BALL;
                        spawnPowerUp(block.getX(), block.getY(), type);
                    }
                    else if (r < 70) {
                        int t = rand() % 3;
                        if (t == 0) type = PowerUpType::SHORT_PADDLE;
                        else if (t == 1) type = PowerUpType::FAST_BALL;
                        else type = PowerUpType::SLOW_PADDLE;
                        spawnPowerUp(block.getX(), block.getY(), type);
                    }
                }
                break;
            }
        }
    }

    for (auto it = powerUps.begin(); it != powerUps.end(); ) {
        bool collected = false;
        if (state == GameState::MULTIPLAYER_PLAYING) {
            if (it->getBounds().intersects(paddleLeft.getBounds())) { applyPowerUp(it->getType(), true); collected = true; }
            else if (it->getBounds().intersects(paddleRight.getBounds())) { applyPowerUp(it->getType(), false); collected = true; }
        }
        else {
            if (it->getBounds().intersects(paddle.getBounds())) { applyPowerUp(it->getType(), true); collected = true; }
        }
        if (collected || it->getY() > 600) it = powerUps.erase(it);
        else ++it;
    }
}

int Game::countAliveBlocks() const {
    int cnt = 0;
    for (const auto& b : blocks)
        if (b.isAlive() && !b.isIndestructible()) cnt++;
    return cnt;
}

void Game::spawnPowerUp(float x, float y, PowerUpType type) {
    powerUps.emplace_back(x, y, type);
}

void Game::applyPowerUp(PowerUpType type, bool forLeftPaddle) {
    switch (type) {
    case PowerUpType::EXTEND_PADDLE:
        if (state == GameState::MULTIPLAYER_PLAYING) {
            if (forLeftPaddle) paddleLeft.widen();
            else paddleRight.widen();
        }
        else paddle.widen();
        break;
    case PowerUpType::MULTI_BALL:
    {
        size_t maxBalls = 8;
        std::vector<Ball> newBalls;
        newBalls.reserve(maxBalls);
        for (auto& ball : balls) {
            newBalls.push_back(ball);
            if (newBalls.size() < maxBalls) {
                Ball newBall(ball.getX(), ball.getY());
                float vx = ball.getVelocityX() + ((rand() % 3) - 1) * 0.5f;
                float vy = -std::abs(ball.getVelocityY());
                newBall.setVelocity(vx, vy);
                newBalls.push_back(newBall);
            }
        }
        balls = newBalls;
    }
    break;
    case PowerUpType::SHORT_PADDLE:
        if (state == GameState::MULTIPLAYER_PLAYING) {
            if (forLeftPaddle) paddleLeft.setShortPaddle(true, 8.0f);
            else paddleRight.setShortPaddle(true, 8.0f);
        }
        else paddle.setShortPaddle(true, 8.0f);
        break;
    case PowerUpType::FAST_BALL:
        fastBallMode = true;
        fastBallTimer = 7.0f;
        break;
    case PowerUpType::SLOW_PADDLE:
        if (state == GameState::MULTIPLAYER_PLAYING) {
            if (forLeftPaddle) paddleLeft.setSlowPaddle(true, 7.0f);
            else paddleRight.setSlowPaddle(true, 7.0f);
        }
        else paddle.setSlowPaddle(true, 7.0f);
        break;
    }
}

void Game::render() {
    if (state == GameState::MAIN_MENU) { drawMenu(); return; }
    if (state == GameState::MULTIPLAYER_MENU) { drawMultiplayerMenu(); return; }
    if (state == GameState::PAUSE) { drawPauseMenu(); return; }
    if (state == GameState::GAME_OVER) { drawGameOver(); return; }
    if (state == GameState::YOU_WIN) { drawYouWin(); return; }

    window.clear();
    window.draw(backgroundGame);

    if (state == GameState::MULTIPLAYER_PLAYING) {
        paddleLeft.draw(window);
        paddleRight.draw(window);
    }
    else {
        paddle.draw(window);
    }
    for (auto& ball : balls) ball.draw(window);
    for (auto& block : blocks) if (block.isAlive()) block.draw(window);
    for (auto& pu : powerUps) pu.draw(window);

    std::string line1 = "Lives: " + std::to_string(lives);
    std::string line2 = "Score: " + std::to_string(score);
    std::string line3 = "Blocks: " + std::to_string(countAliveBlocks());

    if (state == GameState::MULTIPLAYER_PLAYING)
        line3 += "  Level: " + std::to_string(multiplayerLevel) + "/3";
    else if (state == GameState::SECRET_LEVEL)
        line3 += "  SECRET LEVEL!";
    else if (state == GameState::SECRET_UNLOCKING)
        line3 += "  Level: 5/5";
    else
        line3 += "  Level: " + std::to_string(currentLevel) + "/5";

    sf::Text info1, info2, info3;
    info1.setFont(font); info1.setCharacterSize(18); info1.setFillColor(sf::Color::White);
    info1.setString(line1); info1.setPosition(10, 10); window.draw(info1);

    info2.setFont(font); info2.setCharacterSize(18); info2.setFillColor(sf::Color::White);
    info2.setString(line2); info2.setPosition(10, 32); window.draw(info2);

    info3.setFont(font); info3.setCharacterSize(18); info3.setFillColor(sf::Color::White);
    info3.setString(line3); info3.setPosition(10, 54); window.draw(info3);

    int yOff = 10;
    if (fastBallMode) {
        sf::Text t; t.setFont(font); t.setCharacterSize(13); t.setFillColor(sf::Color(255, 165, 0));
        t.setString("FAST BALLS");
        sf::FloatRect b = t.getLocalBounds();
        t.setPosition(790 - b.width, yOff); window.draw(t); yOff += 20;
    }
    if ((state == GameState::MULTIPLAYER_PLAYING && paddleLeft.isSlow()) || (state != GameState::MULTIPLAYER_PLAYING && paddle.isSlow())) {
        sf::Text t; t.setFont(font); t.setCharacterSize(13); t.setFillColor(sf::Color(128, 128, 128));
        t.setString("SLOW PADDLE");
        sf::FloatRect b = t.getLocalBounds();
        t.setPosition(790 - b.width, yOff); window.draw(t); yOff += 20;
    }
    if ((state == GameState::MULTIPLAYER_PLAYING && paddleLeft.isShort()) || (state != GameState::MULTIPLAYER_PLAYING && paddle.isShort())) {
        sf::Text t; t.setFont(font); t.setCharacterSize(13); t.setFillColor(sf::Color(128, 0, 0));
        t.setString("SHORT PADDLE");
        sf::FloatRect b = t.getLocalBounds();
        t.setPosition(790 - b.width, yOff); window.draw(t); yOff += 20;
    }

    if (state == GameState::SECRET_UNLOCKING) {
        sf::Text secretMsg;
        secretMsg.setFont(font);
        secretMsg.setCharacterSize(18);
        secretMsg.setFillColor(sf::Color::Yellow);
        secretMsg.setString("HIT RIGHT WALL 3x!  Time: " + std::to_string((int)secretTimer) + "s  Hits: " + std::to_string(secretHitCount) + "/3");
        sf::FloatRect mb = secretMsg.getLocalBounds();
        secretMsg.setPosition(400 - mb.width / 2, 280);
        window.draw(secretMsg);
    }
    window.display();
}