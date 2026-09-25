#include "Game.h"
#include <iostream>
#include <algorithm>

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
    scoreMultiplier(1),
    doubleScoreEndTime(0.f),
    bannerMessage(""),
    gameTime(0.f),
    bannerMessageEndTime(0.f),
    dropInterval(0.5f),
    holdOffset({ 50.f, 80.f }),
    boardOffset({ 270.f, 50.f }),
    queueOffset({ 580.f, 80.f }),
    tileSize(26.f),
    playButton("assets/buttons/boton_jugar_0.png", "assets/buttons/boton_jugar_1.png", { 140.f, 380.f }),
    exitButton("assets/buttons/boton_salir_0.png", "assets/buttons/boton_salir_1.png", { 140.f, 445.f }),
    bubbleButton("assets/buttons/boton_burbuja_0.png", "assets/buttons/boton_burbuja_1.png", { 65.f, 250.f }),
    quickSortButton("assets/buttons/boton_quicksort_0.png", "assets/buttons/boton_quicksort_1.png", { 215.f, 250.f })
{
    if (!textureManager.load("assets/bloques_De_Colores.png")) {
        std::cerr << "Error: No se pudo cargar assets/bloques_De_Colores.png\n";
    }

    if (!font.openFromFile("assets/fonts/GradvisRegular-lxoyd.ttf")) {
        std::cerr << "Error: No se pudo cargar assets/fonts/GradvisRegular-lxoyd.ttf\n";
    }

    nameField.init(font, { 110.f, 150.f }, { 220.f, 35.f });
    nameField.setText("Jugador1");

    scoreTable.setAlgorithm(selectedAlgorithm);
}

void Game::initScheduledEvents() {
    eventQueue.clear();

    // Evento Tipo 1: Aumento de velocidad a los 25s
    eventQueue.enqueue(Event(EventType::SPEED_UP, 25.f, 0.f, "Velocidad Aumentada"));

    // Evento Tipo 2: Puntos Dobles x2 a los 45s durante 15s
    eventQueue.enqueue(Event(EventType::DOUBLE_SCORE, 45.f, 15.f, "Puntos x2 (15s)"));

    // Evento Tipo 3: Limpieza de la fila inferior a los 75s
    eventQueue.enqueue(Event(EventType::CLEAR_BOTTOM_ROW, 75.f, 0.f, "Limpieza Fila Inferior"));

    // Evento adicional: Aumento de velocidad extra a los 100s
    eventQueue.enqueue(Event(EventType::SPEED_UP, 100.f, 0.f, "Super Velocidad"));
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
                scoreMultiplier = 1;
                dropInterval = 0.5f;
                isReplaying = false;
                bannerMessage = "";

                initScheduledEvents();
                deltaClock.restart();

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

    float dt = deltaClock.restart().asSeconds();
    if (!isReplaying) {
        gameTime += dt;
    }

    if (isReplaying) {
        if (replayClock.getElapsedTime().asSeconds() >= 0.10f) {
            if (currentReplayNode) {
                restoreSnapshot(currentReplayNode->snapshot);
                currentReplayNode = currentReplayNode->next;
            }
            replayClock.restart();
        }
        return;
    }

    float currentGameTime = gameTime;

    // 1. Revision y disparo de la Cola de Eventos Programados
    auto nextEvent = eventQueue.peek();
    if (nextEvent.has_value() && currentGameTime >= nextEvent->triggerTime) {
        Event ev = eventQueue.dequeue();

        if (ev.type == EventType::SPEED_UP) {
            dropInterval = std::max(0.12f, dropInterval - 0.12f);
            bannerMessage = "EVENTO: VELOCIDAD AUMENTADA!";
        }
        else if (ev.type == EventType::DOUBLE_SCORE) {
            scoreMultiplier = 2;
            doubleScoreEndTime = currentGameTime + ev.duration;
            bannerMessage = "EVENTO: PUNTOS X2 ACTIVADOS!";
        }
        else if (ev.type == EventType::CLEAR_BOTTOM_ROW) {
            board.clearBottomRow();
            bannerMessage = "EVENTO: FILA INFERIOR LIMPIADA!";
        }

        bannerMessageEndTime = currentGameTime + 4.f; // Mostrar el mensaje por 4s
    }

    // Expiracion del bonificador de Puntos x2
    if (scoreMultiplier > 1 && currentGameTime >= doubleScoreEndTime) {
        scoreMultiplier = 1;
    }

    // 2. Caida automatica por gravedad
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

    // Tabla de clasificaciones (Top 10)
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

    // Dibujar proximos eventos en la UI lateral derecha
    if (!isReplaying) {
        eventQueue.drawUI(window, font, { queueOffset.x, queueOffset.y + 310.f });
    }

    sf::Text nameText(font, "JUGADOR: " + playerName, 16);
    nameText.setFillColor(sf::Color::White);
    nameText.setPosition({ holdOffset.x, holdOffset.y + 120.f });
    window.draw(nameText);

    std::string scoreStr = "PUNTOS: " + std::to_string(currentScore);
    if (scoreMultiplier > 1) scoreStr += " (x2!)";
    sf::Text scoreText(font, scoreStr, 18);
    scoreText.setFillColor(scoreMultiplier > 1 ? sf::Color::Green : sf::Color::Yellow);
    scoreText.setPosition({ holdOffset.x, holdOffset.y + 150.f });
    window.draw(scoreText);

    // Banner de notificacion de evento activo
    float currentGameTime = gameTime;
    if (!bannerMessage.empty() && currentGameTime < bannerMessageEndTime) {
        sf::Text bannerText(font, bannerMessage, 16);
        bannerText.setFillColor(sf::Color::Cyan);
        bannerText.setPosition({ boardOffset.x - 10.f, boardOffset.y - 45.f });
        window.draw(bannerText);
    }

    if (isReplaying) {
        sf::Text replayText(font, "MODO REPLAY (ESC para Menu)", 18);
        replayText.setFillColor(sf::Color::Yellow);
        replayText.setPosition({ boardOffset.x - 20.f, boardOffset.y - 25.f });
        window.draw(replayText);
    }
    else {
        sf::Text controlsText(font, "Z: Undo | R: Redo | P: Replay | ESC: Menu", 13);
        controlsText.setFillColor(sf::Color::White);
        controlsText.setPosition({ boardOffset.x - 30.f, boardOffset.y - 25.f });
        window.draw(controlsText);
    }

    window.display();
}

void Game::addScoreForLines(int linesCleared) {
    int baseScore = 0;
    if (linesCleared == 1) baseScore = 100;
    else if (linesCleared == 2) baseScore = 300;
    else if (linesCleared == 3) baseScore = 500;
    else if (linesCleared >= 4) baseScore = 800;

    currentScore += baseScore * scoreMultiplier;
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
    snap.eventQueueState = eventQueue.getQueueState();
    snap.gameTime = gameTime;
    snap.canHold = canHold;
    snap.score = currentScore;
    return snap;
}

void Game::restoreSnapshot(const GameSnapshot& snapshot) {
    board.setGrid(snapshot.grid);
    currentPiece = snapshot.currentPiece;
    holdStack.setHoldPiece(snapshot.holdPiece);
    pieceQueue.setQueueState(snapshot.queueState);
    eventQueue.setQueueState(snapshot.eventQueueState);
    gameTime = snapshot.gameTime;
    canHold = snapshot.canHold;
    currentScore = snapshot.score;
}

void Game::startReplay() {
    if (movementList.getHead() == nullptr) return;
    isReplaying = true;
    currentReplayNode = movementList.getHead();
    restoreSnapshot(currentReplayNode->snapshot);
    replayClock.restart();
}