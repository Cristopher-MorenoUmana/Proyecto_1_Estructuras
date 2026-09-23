#include "PieceQueue.h"
#include <array>
#include <algorithm>
#include <random>
#include <chrono>

PieceQueue::PieceQueue() : front(nullptr), back(nullptr), count(0) {
    generateBag();
    generateBag();
}

PieceQueue::~PieceQueue() {
    clear();
}

void PieceQueue::clear() {
    while (front != nullptr) {
        PieceNode* temp = front;
        front = front->next;
        delete temp;
    }
    back = nullptr;
    count = 0;
}

void PieceQueue::enqueue(const Piece& piece) {
    PieceNode* newNode = new PieceNode(piece);
    if (isEmpty()) {
        front = newNode;
        back = newNode;
    }
    else {
        back->next = newNode;
        back = newNode;
    }
    count++;
}

Piece PieceQueue::dequeue() {
    if (isEmpty()) {
        return Piece(PieceType::I, 0, 3);
    }

    PieceNode* temp = front;
    Piece poppedPiece = temp->piece;

    front = front->next;
    if (front == nullptr) {
        back = nullptr;
    }

    delete temp;
    count--;

    if (count < 7) {
        generateBag();
    }

    return poppedPiece;
}

bool PieceQueue::isEmpty() const {
    return front == nullptr;
}

void PieceQueue::generateBag() {
    std::array<PieceType, 7> types = {
        PieceType::I, PieceType::O, PieceType::T,
        PieceType::S, PieceType::Z, PieceType::J, PieceType::L
    };

    unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(types.begin(), types.end(), std::default_random_engine(seed));

    for (PieceType type : types) {
        enqueue(Piece(type, 0, 3));
    }
}

std::vector<Piece> PieceQueue::getQueueState() const {
    std::vector<Piece> pieces;
    PieceNode* current = front;
    while (current) {
        pieces.push_back(current->piece);
        current = current->next;
    }
    return pieces;
}

void PieceQueue::setQueueState(const std::vector<Piece>& pieces) {
    clear(); // Liberar nodos de la memoria directamente sin llamar a dequeue()
    for (const auto& piece : pieces) {
        enqueue(piece);
    }
}

void PieceQueue::drawNext(sf::RenderWindow& window, const TextureManager& textureManager, const sf::Font& font, sf::Vector2f position, float tileSize, int amount) const {
    sf::Text titleText(font, "SIGUIENTE", 18);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition({ position.x + 10.f, position.y - 28.f });
    window.draw(titleText);

    float boxWidth = 5.5f * tileSize;
    float boxHeight = (amount * 3.2f + 0.5f) * tileSize;

    sf::RectangleShape previewBox(sf::Vector2f(boxWidth, boxHeight));
    previewBox.setPosition(position);
    previewBox.setFillColor(sf::Color(20, 20, 30, 230));
    previewBox.setOutlineThickness(2.f);
    previewBox.setOutlineColor(sf::Color(200, 200, 200));
    window.draw(previewBox);

    sf::RectangleShape divider(sf::Vector2f(boxWidth - 20.f, 2.f));
    divider.setPosition({ position.x + 10.f, position.y + (3.3f * tileSize) });
    divider.setFillColor(sf::Color(100, 100, 120, 180));
    window.draw(divider);

    PieceNode* current = front;
    int drawn = 0;

    while (current && drawn < amount) {
        Piece previewPiece(current->piece.getType(), 0, 0);

        sf::Vector2f pieceOffset = {
            position.x + (0.8f * tileSize),
            position.y + (0.5f * tileSize) + (drawn * 3.2f * tileSize)
        };

        float currentTileScale = (drawn == 0) ? (tileSize * 0.85f) : (tileSize * 0.70f);
        previewPiece.draw(window, textureManager, pieceOffset, currentTileScale);

        current = current->next;
        drawn++;
    }
}