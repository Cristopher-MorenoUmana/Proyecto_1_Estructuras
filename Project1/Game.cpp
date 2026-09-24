#include "Game.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode({ 850, 650 }), "Tetris"),
    state(GameState::MENU),
    selectedAlgorithm(SortAlgorithm::QUICK_SORT),
    playerName("Jugador1"),
    board(20),
    currentPiece(PieceType::I, 0, 3),
    currentScore(0),
    canHold(true),
    isReplaying(false),
    currentReplayNode(nullptr),
    dropInterval(0.5f),
    holdOffset({ 50.f, 80.f }),
    boardOffset({ 270.f, 50.f }),
    queueOffset({ 580.f, 80.f }),
    tileSize(26.f),
    // Posiciones calculadas para centrar todos los botones en la columna izquierda (X = 220)
    playButton("assets/buttons/boton_jugar_0.png", "assets/buttons/boton_jugar_1.png", { 140.f, 380.f }),
    exitButton("assets/buttons/boton_salir_0.png", "assets/buttons/boton_salir_1.png", { 140.f, 445.f }),
    bubbleButton("assets/buttons/boton_burbuja_0.png", "assets/buttons/boton_burbuja_1.png", { 65.f, 250.f }),
    quickSortButton("assets/buttons/boton_quicksort_0.png", "assets/buttons/boton_quicksort_1.png", { 215.f, 250.f })
{
    if (!textureManager.load("assets/bloques_De_Colores.png")) {
        std::cerr << "Error: No se pudo cargar assets/bloques_De_Colores.png\n";
    }

    // Nueva fuente configurada
    if (!font.openFromFile("assets/fonts/GradvisRegular-lxoyd.ttf")) {
        std::cerr << "Error: No se pudo cargar assets/fonts/GradvisRegular-lxoyd.ttf\n";
    }

    // Escalado de equivalencia visual para que todos midan ~140x45 px en pantalla
    //playButton.setScale({ 4.85f, 2.65f });      // 33x17 -> 160x45
    //exitButton.setScale({ 2.22f, 1.28f });      // 72x35 -> 160x45
    //bubbleButton.setScale({ 1.14f, 1.05f });    // 114x38 -> 130x40
    //quickSortButton.setScale({ 0.98f, 1.00f }); // 132x40 -> 130x40

    // Campo de texto de nombre centrado
    nameField.init(font, { 110.f, 150.f }, { 220.f, 35.f });
    nameField.setText("Jugador1");

    // Cargar mejores puntajes del archivo
    scoreTable.setAlgorithm(selectedAlgorithm);
}

void Game::start() {
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (state == GameState::MENU) {
                handleMenuInput(*event);
            }
            else {
                handlePlayingInput(*event);
            }
        }

        if (state == GameState::MENU) {
            updateMenu();
            renderMenu();
        }
        else {
            updatePlaying();
            renderPlaying();
        }
    }
}

void Game::handleMenuInput(const sf::Event& event) {
    nameField.handleEvent(event, window);

    if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButton->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (playButton.isOver(mousePos)) {
                if (!nameField.getText().empty()) {
                    playerName = nameField.getText();
                }
                scoreTable.setAlgorithm(selectedAlgorithm);
                scoreTable.sort();

                board.reset();
                holdStack.clear();
                movementList.clear();
                currentScore = 0;
                isReplaying = false;

                spawnNewPiece();
                movementList.record(createSnapshot(), MovementType::START);

                state = GameState::PLAYING;
            }
            else if (exitButton.isOver(mousePos)) {
                window.close();
            }
            else if (bubbleButton.isOver(mousePos)) {
                selectedAlgorithm = SortAlgorithm::BUBBLE_SORT;
                scoreTable.setAlgorithm(selectedAlgorithm);
                scoreTable.sort();
            }
            else if (quickSortButton.isOver(mousePos)) {
                selectedAlgorithm = SortAlgorithm::QUICK_SORT;
                scoreTable.setAlgorithm(selectedAlgorithm);
                scoreTable.sort();
            }
        }
    }
}

void Game::handlePlayingInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (isReplaying) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                state = GameState::MENU;
            }
            return;
        }

        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            state = GameState::MENU;
            return;
        }

        if (keyPressed->code == sf::Keyboard::Key::Z) {
            auto prevSnap = movementList.undo();
            if (prevSnap.has_value()) {
                restoreSnapshot(prevSnap.value());
            }
        }
        else if (keyPressed->code == sf::Keyboard::Key::R) {
            auto nextSnap = movementList.redo();
            if (nextSnap.has_value()) {
                restoreSnapshot(nextSnap.value());
            }
        }
        else if (keyPressed->code == sf::Keyboard::Key::P) {
            startReplay();
        }
        else if (keyPressed->code == sf::Keyboard::Key::Left) {
            Piece testPiece = currentPiece;
            testPiece.move(0, -1);
            if (board.isPositionValid(testPiece)) {
                currentPiece = testPiece;
                movementList.record(createSnapshot(), MovementType::MOVE_LEFT);
            }
        }
        else if (keyPressed->code == sf::Keyboard::Key::Right) {
            Piece testPiece = currentPiece;
            testPiece.move(0, 1);
            if (board.isPositionValid(testPiece)) {
                currentPiece = testPiece;
                movementList.record(createSnapshot(), MovementType::MOVE_RIGHT);
            }
        }
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
                int cleared = board.clearFullLines();
                if (cleared > 0) {
                    addScoreForLines(cleared);
                }
                movementList.record(createSnapshot(), MovementType::LOCK_PIECE);
                spawnNewPiece();
            }
        }
        else if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::Space) {
            Piece testPiece = currentPiece;
            testPiece.rotate();
            if (board.isPositionValid(testPiece)) {
                currentPiece = testPiece;
                movementList.record(createSnapshot(), MovementType::ROTATE);
            }
        }
        else if (keyPressed->code == sf::Keyboard::Key::C || keyPressed->code == sf::Keyboard::Key::LShift) {
            useHold();
        }
    }
}

void Game::updateMenu() {
    playButton.update(window);
    exitButton.update(window);
    bubbleButton.update(window);
    quickSortButton.update(window);
}

void Game::updatePlaying() {
    if (isReplaying) {
        if (replayClock.getElapsedTime().asSeconds() >= 0.3f) {
            if (currentReplayNode) {
                restoreSnapshot(currentReplayNode->snapshot);
                currentReplayNode = currentReplayNode->next;
            }
            replayClock.restart();
        }
        return;
    }

    if (dropClock.getElapsedTime().asSeconds() >= dropInterval) {
        Piece testPiece = currentPiece;
        testPiece.move(1, 0);

        if (board.isPositionValid(testPiece)) {
            currentPiece = testPiece;
        }
        else {
            board.lockPiece(currentPiece);
            int cleared = board.clearFullLines();
            if (cleared > 0) {
                addScoreForLines(cleared);
            }
            movementList.record(createSnapshot(), MovementType::LOCK_PIECE);
            spawnNewPiece();
        }

        dropClock.restart();
    }
}

void Game::renderMenu() {
    window.clear(sf::Color(20, 20, 30));

    // Panel Izquierdo: Opciones y Botones
    sf::Text titleText(font, "TETRIS", 38);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition({ 160.f, 35.f });
    window.draw(titleText);

    sf::Text nameLabel(font, "Nombre del Jugador:", 16);
    nameLabel.setFillColor(sf::Color::White);
    nameLabel.setPosition({ 120.f, 120.f });
    window.draw(nameLabel);

    nameField.draw(window);

    sf::Text algoLabel(font, "Algoritmo de Ordenamiento:", 16);
    algoLabel.setFillColor(sf::Color::White);
    algoLabel.setPosition({ 100.f, 215.f });
    window.draw(algoLabel);

    bubbleButton.draw(window);
    quickSortButton.draw(window);

    sf::Text selectedLabel(font, (selectedAlgorithm == SortAlgorithm::BUBBLE_SORT ? "Seleccionado: Burbuja" : "Seleccionado: QuickSort"), 14);
    selectedLabel.setFillColor(sf::Color::Cyan);
    selectedLabel.setPosition({ 110.f, 310.f });
    window.draw(selectedLabel);

    playButton.draw(window);
    exitButton.draw(window);

    // Panel Derecho: Tabla de Clasificaciones (Top 10)
    sf::RectangleShape boardBox(sf::Vector2f(360.f, 540.f));
    boardBox.setPosition({ 440.f, 40.f });
    boardBox.setFillColor(sf::Color(10, 10, 20, 220));
    boardBox.setOutlineThickness(2.f);
    boardBox.setOutlineColor(sf::Color(200, 200, 200));
    window.draw(boardBox);

    sf::Text leaderTitle(font, "MEJORES PUNTAJES", 20);
    leaderTitle.setFillColor(sf::Color::Yellow);
    leaderTitle.setStyle(sf::Text::Bold);
    leaderTitle.setPosition({ 490.f, 60.f });
    window.draw(leaderTitle);

    sf::Text algoUsedText(font, (selectedAlgorithm == SortAlgorithm::BUBBLE_SORT ? "[Ordenado por Burbuja]" : "[Ordenado por QuickSort]"), 12);
    algoUsedText.setFillColor(sf::Color::Cyan);
    algoUsedText.setPosition({ 510.f, 95.f });
    window.draw(algoUsedText);

    const auto& records = scoreTable.getRecords();
    float startY = 135.f;
    int rank = 1;

    for (const auto& rec : records) {
        if (rank > 10) break;

        std::string lineStr = std::to_string(rank) + ". " + rec.name;
        sf::Text nameText(font, lineStr, 15);
        nameText.setFillColor(sf::Color::White);
        nameText.setPosition({ 470.f, startY });
        window.draw(nameText);

        sf::Text scoreValText(font, std::to_string(rec.score) + " pts", 15);
        scoreValText.setFillColor(sf::Color::Yellow);
        scoreValText.setPosition({ 680.f, startY });
        window.draw(scoreValText);

        startY += 38.f;
        rank++;
    }

    window.display();
}

void Game::renderPlaying() {
    window.clear(sf::Color(128, 128, 128));

    holdStack.drawHold(window, textureManager, font, holdOffset, tileSize);
    board.draw(window, textureManager, boardOffset, tileSize);

    if (!isReplaying) {
        currentPiece.draw(window, textureManager, boardOffset, tileSize);
    }

    pieceQueue.drawNext(window, textureManager, font, queueOffset, tileSize, 3);

    sf::Text nameText(font, "JUGADOR: " + playerName, 16);
    nameText.setFillColor(sf::Color::White);
    nameText.setPosition({ holdOffset.x, holdOffset.y + 120.f });
    window.draw(nameText);

    sf::Text scoreText(font, "PUNTOS: " + std::to_string(currentScore), 18);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition({ holdOffset.x, holdOffset.y + 150.f });
    window.draw(scoreText);

    if (isReplaying) {
        sf::Text replayText(font, "MODO REPLAY (ESC para Menu)", 18);
        replayText.setFillColor(sf::Color::Yellow);
        replayText.setPosition({ boardOffset.x - 20.f, boardOffset.y - 35.f });
        window.draw(replayText);
    }
    else {
        sf::Text controlsText(font, "Z: Undo | R: Redo | P: Replay | ESC: Menu", 14);
        controlsText.setFillColor(sf::Color::White);
        controlsText.setPosition({ boardOffset.x - 40.f, boardOffset.y - 30.f });
        window.draw(controlsText);
    }

    window.display();
}

void Game::addScoreForLines(int linesCleared) {
    if (linesCleared == 1) currentScore += 100;
    else if (linesCleared == 2) currentScore += 300;
    else if (linesCleared == 3) currentScore += 500;
    else if (linesCleared >= 4) currentScore += 800;
}

void Game::handleGameOver() {
    if (currentScore > 0) {
        scoreTable.addScore(playerName, currentScore);
    }
    startReplay();
}

void Game::spawnNewPiece() {
    currentPiece = pieceQueue.dequeue();
    canHold = true;

    if (!board.isPositionValid(currentPiece)) {
        handleGameOver();
    }
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

GameSnapshot Game::createSnapshot() const {
    GameSnapshot snap;
    snap.grid = board.getGrid();
    snap.currentPiece = currentPiece;
    snap.holdPiece = holdStack.peek();
    snap.queueState = pieceQueue.getQueueState();
    snap.canHold = canHold;
    return snap;
}

void Game::restoreSnapshot(const GameSnapshot& snapshot) {
    board.setGrid(snapshot.grid);
    currentPiece = snapshot.currentPiece;
    holdStack.setHoldPiece(snapshot.holdPiece);
    pieceQueue.setQueueState(snapshot.queueState);
    canHold = snapshot.canHold;
}

void Game::startReplay() {
    if (movementList.getHead() == nullptr) return;
    isReplaying = true;
    currentReplayNode = movementList.getHead();
    restoreSnapshot(currentReplayNode->snapshot);
    replayClock.restart();
}