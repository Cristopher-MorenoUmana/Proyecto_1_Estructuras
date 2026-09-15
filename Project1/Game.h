#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include "TextureManager.h"
#include "Piece.h"

class Game {
private:
    sf::RenderWindow window;
    TextureManager textureManager;
    std::vector<Piece> testPieces;

    void handleInput();
    void update();
    void render();

public:
    Game();
    void start();
};