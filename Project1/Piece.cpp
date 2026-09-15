#include "Piece.h"

// Desplazamientos relativos (fila, columna) para cada tipo de pieza
static const sf::Vector2i PIECE_SHAPES[7][4][4] = {
    // PieceType::I
    {
        {{1,0}, {1,1}, {1,2}, {1,3}},
        {{0,2}, {1,2}, {2,2}, {3,2}},
        {{2,0}, {2,1}, {2,2}, {2,3}},
        {{0,1}, {1,1}, {2,1}, {3,1}}
    },
    // PieceType::O
    {
        {{0,0}, {0,1}, {1,0}, {1,1}},
        {{0,0}, {0,1}, {1,0}, {1,1}},
        {{0,0}, {0,1}, {1,0}, {1,1}},
        {{0,0}, {0,1}, {1,0}, {1,1}}
    },
    // PieceType::T
    {
        {{0,1}, {1,0}, {1,1}, {1,2}},
        {{0,1}, {1,1}, {1,2}, {2,1}},
        {{1,0}, {1,1}, {1,2}, {2,1}},
        {{0,1}, {1,0}, {1,1}, {2,1}}
    },
    // PieceType::S
    {
        {{0,1}, {0,2}, {1,0}, {1,1}},
        {{0,1}, {1,1}, {1,2}, {2,2}},
        {{1,1}, {1,2}, {2,0}, {2,1}},
        {{0,0}, {1,0}, {1,1}, {2,1}}
    },
    // PieceType::Z
    {
        {{0,0}, {0,1}, {1,1}, {1,2}},
        {{0,2}, {1,1}, {1,2}, {2,1}},
        {{1,0}, {1,1}, {2,1}, {2,2}},
        {{0,1}, {1,0}, {1,1}, {2,0}}
    },
    // PieceType::J
    {
        {{0,0}, {1,0}, {1,1}, {1,2}},
        {{0,1}, {0,2}, {1,1}, {2,1}},
        {{1,0}, {1,1}, {1,2}, {2,2}},
        {{0,1}, {1,1}, {2,0}, {2,1}}
    },
    // PieceType::L
    {
        {{0,2}, {1,0}, {1,1}, {1,2}},
        {{0,1}, {1,1}, {2,1}, {2,2}},
        {{1,0}, {1,1}, {1,2}, {2,0}},
        {{0,0}, {0,1}, {1,1}, {2,1}}
    }
};

BlockColor Piece::getDefaultColor(PieceType pType) {
    switch (pType) {
    case PieceType::I: return BlockColor::SKYBLUE;
    case PieceType::O: return BlockColor::YELLOW;
    case PieceType::T: return BlockColor::PURPLE;
    case PieceType::S: return BlockColor::GREEN;
    case PieceType::Z: return BlockColor::RED;
    case PieceType::J: return BlockColor::SKYBLUE;
    case PieceType::L: return BlockColor::PINK;
    default: return BlockColor::RED;
    }
}

Piece::Piece(PieceType pType, int pRow, int pColumn)
    : type(pType),
    color(getDefaultColor(pType)),
    orientation(0),
    originRow(pRow),
    originColumn(pColumn)
{}

void Piece::rotate() {
    orientation = (orientation + 1) % 4;
}

void Piece::move(int dRow, int dCol) {
    originRow += dRow;
    originColumn += dCol;
}

std::array<sf::Vector2i, 4> Piece::getCoordinates() const {
    std::array<sf::Vector2i, 4> coords;
    int typeIndex = static_cast<int>(type);

    for (int i = 0; i < 4; ++i) {
        sf::Vector2i offset = PIECE_SHAPES[typeIndex][orientation][i];
        coords[i] = sf::Vector2i(originRow + offset.x, originColumn + offset.y);
    }

    return coords;
}

void Piece::draw(sf::RenderWindow& window, const TextureManager& textureManager, sf::Vector2f boardOffset, float tileSize) const {
    auto coords = getCoordinates();

    // Calculamos la escala basándonos en el área efectiva del bloque (18x15 px)
    // Esto hace que el píxel extra del borde (19x16) se solape sobre el bloque vecino.
    float scaleX = tileSize / 18.0f;
    float scaleY = tileSize / 15.0f;

    for (const auto& pos : coords) {
        Block block(
            textureManager.getTexture(),
            textureManager.getRectangle(color),
            color
        );

        block.setScale({ scaleX, scaleY });

        // La posición en la grilla avanza a pasos de tileSize.
        // Al medir la textura 19x16, el píxel 19 (en X) y el píxel 16 (en Y) 
        // caen exactamente sobre el primer píxel del bloque adyacente.
        float posX = boardOffset.x + (pos.y * tileSize);
        float posY = boardOffset.y + (pos.x * tileSize);

        block.setPosition({ posX, posY });
        block.draw(window);
    }
}