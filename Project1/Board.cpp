#include "Board.h"
#include "Block.h"

Board::Board(int numRows)
    : head(nullptr), tail(nullptr), rowCount(numRows)
{
    for (int i = 0; i < rowCount; ++i) {
        RowNode* newNode = new RowNode();
        if (!head) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
}

Board::~Board() {
    RowNode* current = head;
    while (current) {
        RowNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

void Board::reset() {
    RowNode* current = head;
    while (current) {
        current->row.clear();
        current = current->next;
    }
}

RowNode* Board::getNodeAt(int rowIndex) const {
    if (rowIndex < 0 || rowIndex >= rowCount) return nullptr;

    RowNode* current = head;
    int count = 0;
    while (current && count < rowIndex) {
        current = current->next;
        count++;
    }
    return current;
}

bool Board::isPositionValid(const Piece& piece) const {
    auto coords = piece.getCoordinates();

    for (const auto& pos : coords) {
        int row = pos.x;
        int col = pos.y;

        if (col < 0 || col >= 10) return false;
        if (row >= rowCount) return false;

        if (row >= 0) {
            RowNode* node = getNodeAt(row);
            if (node && node->row.getCell(col).has_value()) {
                return false;
            }
        }
    }
    return true;
}

void Board::lockPiece(const Piece& piece) {
    auto coords = piece.getCoordinates();
    BlockColor color = piece.getColor();

    for (const auto& pos : coords) {
        int row = pos.x;
        int col = pos.y;

        if (row >= 0 && row < rowCount && col >= 0 && col < 10) {
            RowNode* node = getNodeAt(row);
            if (node) {
                node->row.setCell(col, color);
            }
        }
    }
}

int Board::clearFullLines() {
    int linesCleared = 0;
    RowNode* current = tail;

    while (current) {
        RowNode* prevNode = current->prev;

        if (current->row.isFull()) {
            linesCleared++;

            if (current->prev) current->prev->next = current->next;
            if (current->next) current->next->prev = current->prev;

            if (current == tail) tail = current->prev;
            if (current == head) head = current->next;

            delete current;

            RowNode* newHead = new RowNode();
            if (!head) {
                head = newHead;
                tail = newHead;
            }
            else {
                newHead->next = head;
                head->prev = newHead;
                head = newHead;
            }
        }

        current = prevNode;
    }

    return linesCleared;
}

void Board::draw(sf::RenderWindow& window, const TextureManager& textureManager, sf::Vector2f boardOffset, float tileSize) const {
    // 1. Dibujar fondo del tablero con marco
    sf::RectangleShape background(sf::Vector2f(10.f * tileSize, rowCount * tileSize));
    background.setPosition(boardOffset);
    background.setFillColor(sf::Color(20, 20, 30, 230)); // Fondo oscuro semitransparente
    background.setOutlineThickness(3.f);
    background.setOutlineColor(sf::Color(200, 200, 200)); // Borde gris claro
    window.draw(background);

    // 2. Dibujar celdas ocupadas
    float scaleX = tileSize / 18.0f;
    float scaleY = tileSize / 15.0f;

    RowNode* current = head;
    int rowIndex = 0;

    while (current) {
        for (int col = 0; col < 10; ++col) {
            auto cell = current->row.getCell(col);
            if (cell.has_value()) {
                BlockColor color = cell.value();

                Block block(
                    textureManager.getTexture(),
                    textureManager.getRectangle(color),
                    color
                );

                block.setScale({ scaleX, scaleY });
                float posX = boardOffset.x + (col * tileSize);
                float posY = boardOffset.y + (rowIndex * tileSize);

                block.setPosition({ posX, posY });
                block.draw(window);
            }
        }
        current = current->next;
        rowIndex++;
    }
}