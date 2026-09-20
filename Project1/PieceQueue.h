#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.h"
#include "TextureManager.h"

struct PieceNode {
    Piece piece;
    PieceNode* next;

    PieceNode(const Piece& p) : piece(p), next(nullptr) {}
};

class PieceQueue {
private:
    PieceNode* front;
    PieceNode* back;
    int count;

    void generateBag();

public:
    PieceQueue();
    ~PieceQueue();

    PieceQueue(const PieceQueue&) = delete;
    PieceQueue& operator=(const PieceQueue&) = delete;

    void enqueue(const Piece& piece);
    Piece dequeue();
    bool isEmpty() const;
    int getCount() const { return count; }

    // Se agrega el parámetro font para renderizar el título de la vista previa
    void drawNext(sf::RenderWindow& window, const TextureManager& textureManager, const sf::Font& font, sf::Vector2f position, float tileSize, int amount = 3) const;
};