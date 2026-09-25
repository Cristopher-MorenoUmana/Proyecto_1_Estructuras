#pragma once
#include <array>
#include <vector>
#include <optional>
#include "BlockColor.h"
#include "Piece.h"
#include "Event.h"

struct GameSnapshot {
	std::array<std::array<std::optional<BlockColor>, 10>, 20> grid; // Estado de la cuadrícula (20 filas x 10 columnas)
    Piece currentPiece; // Pieza actual en juego
	std::optional<Piece> holdPiece; // Pieza en hold (si existe)
	std::vector<Piece> queueState; // Copia de la cola de piezas
    std::vector<Event> eventQueueState; // Copia de la cola de eventos
    float gameTime; // Tiempo acumulado en ese instante
    bool canHold; 
	int score; // Copia del puntaje en ese instante para evitar bugs de sincronización

    GameSnapshot()
        : currentPiece(PieceType::I, 0, 3), canHold(true), score(0), gameTime(0.f)
    {
		for (auto& row : grid) {
            row.fill(std::nullopt);
        }
    }
};