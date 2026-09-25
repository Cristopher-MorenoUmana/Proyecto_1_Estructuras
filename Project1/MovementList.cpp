#include "MovementList.h"

MovementList::MovementList()
    : head(nullptr), tail(nullptr), current(nullptr), count(0) {}

MovementList::~MovementList() {
    clear();
}

void MovementList::clear() {
    deleteSubtree(head);
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    count = 0;
}

void MovementList::deleteSubtree(MovementNode* node) { // Vaciar lista apartir de un nodo dado
    while (node) {
        MovementNode* nextNode = node->next;
        delete node;
        node = nextNode;
    }
}

void MovementList::record(const GameSnapshot& snapshot, MovementType type) {

	if (current && current->next) { // Si hay movimientos futuros, eliminarlos
        deleteSubtree(current->next);
        current->next = nullptr;
        tail = current;
    }

    MovementNode* newNode = new MovementNode(snapshot, type);

    if (!head) {
        head = newNode;
        tail = newNode;
        current = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
        current = newNode;
    }
    count++;
}

bool MovementList::canUndo() const {
    return current != nullptr && current->prev != nullptr;
}

bool MovementList::canRedo() const {
    return current != nullptr && current->next != nullptr;
}

std::optional<GameSnapshot> MovementList::undo() {
    if (!canUndo()) return std::nullopt;
    current = current->prev;
    return current->snapshot;
}

std::optional<GameSnapshot> MovementList::redo() {
    if (!canRedo()) return std::nullopt;
    current = current->next;
    return current->snapshot;
}