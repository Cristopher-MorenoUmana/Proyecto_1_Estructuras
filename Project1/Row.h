#pragma once
#include <array>
#include <optional>
#include "BlockColor.h"

class Row {
private:
    // Cada fila tiene 10 celdas (std::nullopt representa celda vacía)
    std::array<std::optional<BlockColor>, 10> cells;

public:
    Row();

    bool isFull() const;
    bool isEmpty() const;
    void clear();

    std::optional<BlockColor> getCell(int col) const;
    void setCell(int col, BlockColor color);
    void setCell(int col, std::optional<BlockColor> color);
};
