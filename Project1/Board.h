#pragma once
#include <SFML/Graphics.hpp>
#include "Row.h"
#include "Piece.h"
#include "TextureManager.h"

struct RowNode {
    Row row;
    RowNode* next = nullptr;
    RowNode* prev = nullptr;
};

class Board {
private:
    RowNode* head;
    RowNode* tail;
    int rowCount;

    RowNode* getNodeAt(int rowIndex) const;

public:
    Board(int numRows = 20);
    ~Board();

    // Deshabilitar copia por asignación para evitar punteros colgantes
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    // Método seguro para limpiar el tablero al perder
    void reset();

    bool isPositionValid(const Piece& piece) const;
    void lockPiece(const Piece& piece);
    int clearFullLines();

    void draw(sf::RenderWindow& window, const TextureManager& textureManager, sf::Vector2f boardOffset, float tileSize) const;
};