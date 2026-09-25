#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "PieceType.h"
#include "BlockColor.h"
#include "TextureManager.h"
#include "Block.h"

class Piece {
private:
    PieceType type;
    BlockColor color;
    int orientation;    // 0, 1, 2 o 3 (0°, 90°, 180°, 270°)
    int originRow;      // Fila de origen en la cuadrícula
    int originColumn;   // Columna de origen en la cuadrícula

    // Devuelve el color por defecto según el tipo de pieza
    static BlockColor getDefaultColor(PieceType pType);

public:
    Piece(PieceType pType, int pRow = 0, int pColumn = 3);

    void rotate();
    void move(int dRow, int dCol);
    std::array<sf::Vector2i, 4> getCoordinates() const;

    // Dibuja la pieza compuesta por 4 bloques
    void draw(sf::RenderWindow& window, const TextureManager& textureManager, sf::Vector2f boardOffset, float tileSize) const;

    PieceType getType() const { return type; }
    BlockColor getColor() const { return color; }
    int getRow() const { return originRow; }
    int getColumn() const { return originColumn; }
};