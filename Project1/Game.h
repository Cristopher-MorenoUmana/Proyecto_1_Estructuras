#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "TextureManager.h"
#include "Board.h"
#include "Piece.h"
#include "PieceQueue.h"
#include "HoldStack.h"
#include "MovementList.h"
#include "ScoreTable.h"
#include "Button.h"
#include "TextField.h"

enum class GameState {
    MENU,
    PLAYING,
    REPLAY
};

class Game {
private:
    sf::RenderWindow window;
    TextureManager textureManager;
    sf::Font font;

    GameState state;

    // Componentes del Menu Principal
    TextField nameField;
    Button playButton;
    Button exitButton;
    Button bubbleButton;
    Button quickSortButton;

    SortAlgorithm selectedAlgorithm;
    std::string playerName;

    // Componentes del Juego
    Board board;
    PieceQueue pieceQueue;
    HoldStack holdStack;
    MovementList movementList;
    ScoreTable scoreTable;

    Piece currentPiece;
    int currentScore;

    bool canHold;
    bool isReplaying;
    MovementNode* currentReplayNode;
    sf::Clock replayClock;

    sf::Clock dropClock;
    float dropInterval;

    sf::Vector2f boardOffset;
    sf::Vector2f queueOffset;
    sf::Vector2f holdOffset;
    float tileSize;

    void handleMenuInput(const sf::Event& event);
    void handlePlayingInput(const sf::Event& event);

    void updateMenu();
    void updatePlaying();

    void renderMenu();
    void renderPlaying();

    void spawnNewPiece();
    void useHold();
    void addScoreForLines(int linesCleared);
    void handleGameOver();

    GameSnapshot createSnapshot() const;
    void restoreSnapshot(const GameSnapshot& snapshot);
    void startReplay();

public:
    Game();
    void start();
};