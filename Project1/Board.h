#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <optional>
#include "Row.h"
#include "Piece.h"
#include "TextureManager.h"

// Nodo para la lista enlazada de filas
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

    // Deshabilitar copia por asignación para prevenir punteros colgantes y heap corruption
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    void reset();

    bool isPositionValid(const Piece& piece) const;
    void lockPiece(const Piece& piece);
    int clearFullLines();

    // Métodos para capturar y restaurar la matriz (Requeridos por MovementList)
    std::array<std::array<std::optional<BlockColor>, 10>, 20> getGrid() const;
    void setGrid(const std::array<std::array<std::optional<BlockColor>, 10>, 20>& grid);

    void clearBottomRow(); // Elimina la ultima fila (tail) y agrega una vacia arriba (head)

    void draw(sf::RenderWindow& window, const TextureManager& textureManager, sf::Vector2f boardOffset, float tileSize) const;
};