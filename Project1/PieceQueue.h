#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
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

    void enqueue(const Piece& piece); // Push
	Piece dequeue(); // Pop
    bool isEmpty() const; // vacia
    int getCount() const { return count; }
    void clear(); // Vacía la cola de forma segura sin generar nuevas bolsas

    std::vector<Piece> getQueueState() const;
    void setQueueState(const std::vector<Piece>& pieces);

    void drawNext(sf::RenderWindow& window, const TextureManager& textureManager, const sf::Font& font, sf::Vector2f position, float tileSize, int amount = 3) const;
};