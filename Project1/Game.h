#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "TextureManager.h"
#include "Board.h"
#include "Piece.h"
#include "PieceQueue.h"

class Game {
private:
    sf::RenderWindow window;
    TextureManager textureManager;
    sf::Font font; // Fuente para los textos del juego
    Board board;
    PieceQueue pieceQueue;
    Piece currentPiece;

    sf::Clock dropClock;
    float dropInterval;

    sf::Vector2f boardOffset;
    sf::Vector2f queueOffset;
    float tileSize;

    void handleInput();
    void update();
    void render();
    void spawnNewPiece();

public:
    Game();
    void start();
};