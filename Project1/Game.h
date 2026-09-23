#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "TextureManager.h"
#include "Board.h"
#include "Piece.h"
#include "PieceQueue.h"
#include "HoldStack.h"
#include "MovementList.h"

class Game {
private:
    sf::RenderWindow window;
    TextureManager textureManager;
    sf::Font font;
    Board board;
    PieceQueue pieceQueue;
    HoldStack holdStack;
    MovementList movementList; // Historial doblemente enlazado
    Piece currentPiece;

    bool canHold;
    bool isReplaying; // Bandera para el modo reproducción
    MovementNode* currentReplayNode;
    sf::Clock replayClock;

    sf::Clock dropClock;
    float dropInterval;

    sf::Vector2f boardOffset;
    sf::Vector2f queueOffset;
    sf::Vector2f holdOffset;
    float tileSize;

    void handleInput();
    void update();
    void render();
    void spawnNewPiece();
    void useHold();

    GameSnapshot createSnapshot() const;
    void restoreSnapshot(const GameSnapshot& snapshot);
    void startReplay();

public:
    Game();
    void start();
};