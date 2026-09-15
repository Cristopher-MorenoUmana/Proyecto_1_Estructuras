#include "Game.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode({ 900, 600 }), "Tetris - Prueba de las 7 Piezas")
{
    if (!textureManager.load("assets/bloques_De_Colores.png")) {
        std::cerr << "Error: No se pudo cargar assets/bloques_De_Colores.png\n";
    }

    // Crear las 7 piezas
    PieceType allTypes[] = {
        PieceType::I, PieceType::O, PieceType::T,
        PieceType::S, PieceType::Z, PieceType::J, PieceType::L
    };

    // Ubicar cada pieza en una posición distinta
    for (int i = 0; i < 7; ++i) {
        int row = (i / 4) * 5;      // Fila de la cuadrícula de prueba
        int col = (i % 4) * 5;      // Columna de la cuadrícula de prueba
        testPieces.push_back(Piece(allTypes[i], row, col));
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

        // Al presionar ESPACIO o FLECHA ARRIBA, rotan TODAS las piezas simultáneamente
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Space || keyPressed->code == sf::Keyboard::Key::Up) {
                for (auto& piece : testPieces) {
                    piece.rotate();
                }
            }
        }
    }
}

void Game::update() {}

void Game::render() {
    window.clear(sf::Color(128, 128, 128));

    // Dibujar las 7 piezas con tamaño de celda de 32px
    for (const auto& piece : testPieces) {
        piece.draw(window, textureManager, { 50.f, 50.f }, 32.f);
    }

    window.display();
}