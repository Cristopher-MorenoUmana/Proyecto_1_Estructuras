#include "Game.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode({ 800, 650 }), "Tetris - Vista Previa con Texto"),
    board(20),
    currentPiece(PieceType::I, 0, 3),
    dropInterval(0.5f),
    boardOffset({ 220.f, 50.f }),
    queueOffset({ 510.f, 80.f }), // Ajustado un poco para dar espacio al título
    tileSize(26.f)
{
    if (!textureManager.load("assets/bloques_De_Colores.png")) {
        std::cerr << "Error: No se pudo cargar assets/bloques_De_Colores.png\n";
    }

    // Cargar fuente para las etiquetas de texto
    if (!font.openFromFile("assets/fonts/pix32.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente assets/fonts/pix32.ttf\n";
		window.close(); // Cerrar la ventana si la fuente no se carga
    }

    spawnNewPiece();
}

void Game::spawnNewPiece() {
    // Extraer la siguiente pieza garantizada por la cola
    currentPiece = pieceQueue.dequeue();

    if (!board.isPositionValid(currentPiece)) {
        std::cout << "Game Over! Reiniciando tablero...\n";
        board.reset();
    }
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
            if (keyPressed->code == sf::Keyboard::Key::Left) {
                Piece testPiece = currentPiece;
                testPiece.move(0, -1);
                if (board.isPositionValid(testPiece)) currentPiece = testPiece;
            }
            else if (keyPressed->code == sf::Keyboard::Key::Right) {
                Piece testPiece = currentPiece;
                testPiece.move(0, 1);
                if (board.isPositionValid(testPiece)) currentPiece = testPiece;
            }
            else if (keyPressed->code == sf::Keyboard::Key::Down) {
                Piece testPiece = currentPiece;
                testPiece.move(1, 0);
                if (board.isPositionValid(testPiece)) {
                    currentPiece = testPiece;
                    dropClock.restart();
                }
                else {
                    board.lockPiece(currentPiece);
                    board.clearFullLines();
                    spawnNewPiece();
                }
            }
            else if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::Space) {
                Piece testPiece = currentPiece;
                testPiece.rotate();
                if (board.isPositionValid(testPiece)) currentPiece = testPiece;
            }
        }
    }
}

void Game::update() {
    if (dropClock.getElapsedTime().asSeconds() >= dropInterval) {
        Piece testPiece = currentPiece;
        testPiece.move(1, 0);

        if (board.isPositionValid(testPiece)) {
            currentPiece = testPiece;
        }
        else {
            board.lockPiece(currentPiece);
            board.clearFullLines();
            spawnNewPiece();
        }

        dropClock.restart();
    }
}

void Game::render() {
    window.clear(sf::Color(128, 128, 128));

    // 1. Dibujar el tablero
    board.draw(window, textureManager, boardOffset, tileSize);

    // 2. Dibujar la pieza cayendo
    currentPiece.draw(window, textureManager, boardOffset, tileSize);

    // 3. Dibujar la cola de siguientes piezas pasando la fuente
    pieceQueue.drawNext(window, textureManager, font, queueOffset, tileSize, 3);

    window.display();
}