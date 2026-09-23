#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "Piece.h"
#include "TextureManager.h"

// Nodo dinámico propio para la pila Hold (NodoHold en el diagrama)
struct HoldNode {
    Piece piece;
    HoldNode* next;

    HoldNode(const Piece& p) : piece(p), next(nullptr) {}
};

class HoldStack {
private:
    HoldNode* top;
    int capacity; // Capacidad máxima estricta (1 elemento según el enunciado)
    int count;

public:
    HoldStack();
    ~HoldStack();

    // Prevenir copia por asignación para evitar punteros colgados
    HoldStack(const HoldStack&) = delete;
    HoldStack& operator=(const HoldStack&) = delete;

    bool push(const Piece& piece);
    Piece pop();
    bool isEmpty() const;
    bool isFull() const;
    void clear();

    void setHoldPiece(std::optional<Piece> piece);

    std::optional<Piece> peek() const;

    // Dibujar el panel "EN ESPERA" a la izquierda del tablero
    void drawHold(sf::RenderWindow& window, const TextureManager& textureManager, const sf::Font& font, sf::Vector2f position, float tileSize) const;
};