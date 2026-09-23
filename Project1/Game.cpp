#include "Game.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode({ 850, 650 }), "Tetris - Historial y Replay"),
    board(20),
    currentPiece(PieceType::I, 0, 3),
    canHold(true),
    isReplaying(false),
    currentReplayNode(nullptr),
    dropInterval(0.5f),
    holdOffset({ 50.f, 80.f }),
    boardOffset({ 270.f, 50.f }),
    queueOffset({ 580.f, 80.f }),
    tileSize(26.f)
{
    if (!textureManager.load("assets/bloques_De_Colores.png")) {
        std::cerr << "Error: No se pudo cargar assets/bloques_De_Colores.png\n";
    }

    if (!font.openFromFile("assets/fonts/pix32.ttf")) {
        std::cerr << "Error: No se pudo cargar assets/fonts/pix32.ttf\n";
    }

    spawnNewPiece();
    // Registrar el estado inicial
    movementList.record(createSnapshot(), MovementType::START);
}

GameSnapshot Game::createSnapshot() const {
    GameSnapshot snap;
    snap.grid = board.getGrid();
    snap.currentPiece = currentPiece;
    snap.holdPiece = holdStack.peek();
    snap.queueState = pieceQueue.getQueueState(); // Guardar cola
    snap.canHold = canHold;
    return snap;
}

void Game::restoreSnapshot(const GameSnapshot& snapshot) {
    board.setGrid(snapshot.grid);
    currentPiece = snapshot.currentPiece;
    holdStack.setHoldPiece(snapshot.holdPiece);
    pieceQueue.setQueueState(snapshot.queueState); // Restaurar cola
    canHold = snapshot.canHold;
}

void Game::spawnNewPiece() {
    currentPiece = pieceQueue.dequeue();
    canHold = true;

    if (!board.isPositionValid(currentPiece)) {
        std::cout << "Game Over! Iniciando reproduccion automatica...\n";
        startReplay();
    }
}

void Game::startReplay() {
    if (movementList.getHead() == nullptr) return;
    isReplaying = true;
    currentReplayNode = movementList.getHead();
    restoreSnapshot(currentReplayNode->snapshot);
    replayClock.restart();
}

void Game::useHold() {
    if (!canHold || isReplaying) return;

    if (holdStack.isEmpty()) {
        Piece pieceToHold(currentPiece.getType(), 0, 3);
        holdStack.push(pieceToHold);
        currentPiece = pieceQueue.dequeue();
    }
    else {
        Piece heldPiece = holdStack.pop();
        Piece newHoldPiece(currentPiece.getType(), 0, 3);
        holdStack.push(newHoldPiece);
        currentPiece = Piece(heldPiece.getType(), 0, 3);
    }

    canHold = false;
    dropClock.restart();
    movementList.record(createSnapshot(), MovementType::HOLD);
}

void Game::start() {
    while (window.isOpen()) {
        handleInput();
        update();
        render();
    }
}

void Game::handleInput() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            // Durante el modo Replay no procesamos controles del usuario excepto salir del modo
            if (isReplaying) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    isReplaying = false;
                    board.reset();
                    holdStack.clear();
                    movementList.clear();
                    spawnNewPiece();
                    movementList.record(createSnapshot(), MovementType::START);
                }
                continue;
            }

            // Tecla Z: Deshacer (Undo)
            if (keyPressed->code == sf::Keyboard::Key::Z) {
                auto prevSnap = movementList.undo();
                if (prevSnap.has_value()) {
                    restoreSnapshot(prevSnap.value());
                }
            }
            // Tecla R: Rehacer (Redo)
            else if (keyPressed->code == sf::Keyboard::Key::R) {
                auto nextSnap = movementList.redo();
                if (nextSnap.has_value()) {
                    restoreSnapshot(nextSnap.value());
                }
            }
            // Tecla P: Reproducir partida desde el inicio
            else if (keyPressed->code == sf::Keyboard::Key::P) {
                startReplay();
            }
            // Movimiento Izquierda
            else if (keyPressed->code == sf::Keyboard::Key::Left) {
                Piece testPiece = currentPiece;
                testPiece.move(0, -1);
                if (board.isPositionValid(testPiece)) {
                    currentPiece = testPiece;
                    movementList.record(createSnapshot(), MovementType::MOVE_LEFT);
                }
            }
            // Movimiento Derecha
            else if (keyPressed->code == sf::Keyboard::Key::Right) {
                Piece testPiece = currentPiece;
                testPiece.move(0, 1);
                if (board.isPositionValid(testPiece)) {
                    currentPiece = testPiece;
                    movementList.record(createSnapshot(), MovementType::MOVE_RIGHT);
                }
            }
            // Bajar pieza
            else if (keyPressed->code == sf::Keyboard::Key::Down) {
                Piece testPiece = currentPiece;
                testPiece.move(1, 0);
                if (board.isPositionValid(testPiece)) {
                    currentPiece = testPiece;
                    dropClock.restart();
                    movementList.record(createSnapshot(), MovementType::SOFT_DROP);
                }
                else {
                    board.lockPiece(currentPiece);
                    board.clearFullLines();
                    movementList.record(createSnapshot(), MovementType::LOCK_PIECE);
                    spawnNewPiece();
                }
            }
            // Rotación
            else if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::Space) {
                Piece testPiece = currentPiece;
                testPiece.rotate();
                if (board.isPositionValid(testPiece)) {
                    currentPiece = testPiece;
                    movementList.record(createSnapshot(), MovementType::ROTATE);
                }
            }
            // Hold
            else if (keyPressed->code == sf::Keyboard::Key::C || keyPressed->code == sf::Keyboard::Key::LShift) {
                useHold();
            }
        }
    }
}

void Game::update() {
    // Manejo de la reproducción automática (Replay Mode)
    if (isReplaying) {
        if (replayClock.getElapsedTime().asSeconds() >= 0.3f) {
            if (currentReplayNode) {
                restoreSnapshot(currentReplayNode->snapshot);
                currentReplayNode = currentReplayNode->next;
            }
            else {
                std::cout << "Replay finalizado. Presione ESC para reiniciar.\n";
            }
            replayClock.restart();
        }
        return;
    }

    // Caída automática estándar por gravedad
    if (dropClock.getElapsedTime().asSeconds() >= dropInterval) {
        Piece testPiece = currentPiece;
        testPiece.move(1, 0);

        if (board.isPositionValid(testPiece)) {
            currentPiece = testPiece;
        }
        else {
            board.lockPiece(currentPiece);
            board.clearFullLines();
            movementList.record(createSnapshot(), MovementType::LOCK_PIECE);
            spawnNewPiece();
        }

        dropClock.restart();
    }
}

void Game::render() {
    window.clear(sf::Color(128, 128, 128));

    // 1. Paneles laterales y tablero
    holdStack.drawHold(window, textureManager, font, holdOffset, tileSize);
    board.draw(window, textureManager, boardOffset, tileSize);

    if (!isReplaying) {
        currentPiece.draw(window, textureManager, boardOffset, tileSize);
    }

    pieceQueue.drawNext(window, textureManager, font, queueOffset, tileSize, 3);

    // 2. Indicadores de estado de texto
    if (isReplaying) {
        sf::Text replayText(font, "MODO REPLAY (ESC para salir)", 20);
        replayText.setFillColor(sf::Color::Yellow);
        replayText.setPosition({ boardOffset.x, boardOffset.y - 35.f });
        window.draw(replayText);
    }
    else {
        sf::Text controlsText(font, "Z: Deshacer  |  R: Rehacer  |  P: Replay", 16);
        controlsText.setFillColor(sf::Color::White);
        controlsText.setPosition({ boardOffset.x - 20.f, boardOffset.y - 30.f });
        window.draw(controlsText);
    }

    window.display();
}