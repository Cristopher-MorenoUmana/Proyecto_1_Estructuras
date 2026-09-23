#pragma once
#include <optional>
#include "MovementNode.h"

class MovementList {
private:
    MovementNode* head;
    MovementNode* tail;
    MovementNode* current;
    int count;

    void deleteSubtree(MovementNode* node);

public:
    MovementList();
    ~MovementList();

    // Prevenir copia superficial
    MovementList(const MovementList&) = delete;
    MovementList& operator=(const MovementList&) = delete;

    void record(const GameSnapshot& snapshot, MovementType type);
    bool canUndo() const;
    bool canRedo() const;

    std::optional<GameSnapshot> undo();
    std::optional<GameSnapshot> redo();
    void clear();

    MovementNode* getHead() const { return head; }
    MovementNode* getCurrent() const { return current; }
    int getCount() const { return count; }
};