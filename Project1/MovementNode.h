#pragma once
#include "MovementType.h"
#include "GameSnapshot.h"

struct MovementNode {
    GameSnapshot snapshot;
    MovementType type;
    MovementNode* prev;
    MovementNode* next;

    MovementNode(const GameSnapshot& snap, MovementType t)
        : snapshot(snap), type(t), prev(nullptr), next(nullptr) {}
};