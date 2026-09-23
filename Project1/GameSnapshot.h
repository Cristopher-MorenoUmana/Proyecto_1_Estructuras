#pragma once
#include <array>
#include <vector>
#include <optional>
#include "BlockColor.h"
#include "Piece.h"

struct GameSnapshot {
    std::array<std::array<std::optional<BlockColor>, 10>, 20> grid;
    Piece currentPiece;
    std::optional<Piece> holdPiece;
    std::vector<Piece> queueState; // Estado de la cola de piezas
    bool canHold;

    GameSnapshot() : currentPiece(PieceType::I, 0, 3), canHold(true) {
        for (auto& row : grid) {
            row.fill(std::nullopt);
        }
    }
};