#include "HoldStack.h"

HoldStack::HoldStack()
    : top(nullptr), capacity(1), count(0) {}

HoldStack::~HoldStack() {
    clear();
}

void HoldStack::clear() {
    while (!isEmpty()) {
        pop();
    }
}

bool HoldStack::push(const Piece& piece) {
    if (isFull()) {
        return false; // Capacidad llena (máximo 1 pieza)
    }

    HoldNode* newNode = new HoldNode(piece);
    newNode->next = top;
    top = newNode;
    count++;
    return true;
}

Piece HoldStack::pop() {
    if (isEmpty()) {
        return Piece(PieceType::I, 0, 3);
    }

    HoldNode* temp = top;
    Piece poppedPiece = temp->piece;

    top = top->next;
    delete temp;
    count--;

    return poppedPiece;
}

bool HoldStack::isEmpty() const {
    return top == nullptr;
}

bool HoldStack::isFull() const {
    return count >= capacity;
}

std::optional<Piece> HoldStack::peek() const {
    if (isEmpty()) {
        return std::nullopt;
    }
    return top->piece;
}

void HoldStack::setHoldPiece(std::optional<Piece> piece) {
    clear();
    if (piece.has_value()) {
        push(piece.value());
    }
}

void HoldStack::drawHold(sf::RenderWindow& window, const TextureManager& textureManager, const sf::Font& font, sf::Vector2f position, float tileSize) const {
    // 1. Título del panel "EN ESPERA"
    sf::Text titleText(font, "EN ESPERA", 18);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition({ position.x + 10.f, position.y - 28.f });
    window.draw(titleText);

    // 2. Marco contenedor
    float boxWidth = 5.5f * tileSize;
    float boxHeight = 3.8f * tileSize;

    sf::RectangleShape holdBox(sf::Vector2f(boxWidth, boxHeight));
    holdBox.setPosition(position);
    holdBox.setFillColor(sf::Color(20, 20, 30, 230));
    holdBox.setOutlineThickness(2.f);
    holdBox.setOutlineColor(sf::Color(200, 200, 200));
    window.draw(holdBox);

    // 3. Si hay una pieza guardada, se dibuja centrada dentro del panel
    if (!isEmpty()) {
        Piece holdPiece(top->piece.getType(), 0, 0);

        sf::Vector2f pieceOffset = {
            position.x + (0.8f * tileSize),
            position.y + (0.6f * tileSize)
        };

        holdPiece.draw(window, textureManager, pieceOffset, tileSize * 0.85f);
    }
}