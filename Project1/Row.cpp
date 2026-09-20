#include "Row.h"

Row::Row() {
    // Inicializar las 10 celdas como vacías
    cells.fill(std::nullopt);
}

bool Row::isFull() const {
    for (const auto& cell : cells) {
        if (!cell.has_value()) {
            return false;
        }
    }
    return true;
}

bool Row::isEmpty() const {
    for (const auto& cell : cells) {
        if (cell.has_value()) {
            return false;
        }
    }
    return true;
}

void Row::clear() {
    cells.fill(std::nullopt);
}

std::optional<BlockColor> Row::getCell(int col) const {
    if (col < 0 || col >= 10) return std::nullopt;
    return cells[col];
}

void Row::setCell(int col, BlockColor color) {
    if (col >= 0 && col < 10) {
        cells[col] = color;
    }
}