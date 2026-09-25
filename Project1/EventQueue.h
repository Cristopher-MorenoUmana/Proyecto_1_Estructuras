#pragma once
#include <optional>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Event.h"

struct EventNode {
    Event event;
    EventNode* next;

    EventNode(const Event& e) : event(e), next(nullptr) {}
};

class EventQueue {
private:
    EventNode* front;
    int count;

public:
    EventQueue();
    ~EventQueue();

    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;

    void enqueue(const Event& event);
    Event dequeue();
    std::optional<Event> peek() const;

    bool isEmpty() const;
    int getCount() const { return count; }
    void clear();

    // Guardar y restaurar el estado de la cola para Undo/Redo
    std::vector<Event> getQueueState() const;
    void setQueueState(const std::vector<Event>& events);

    void drawUI(sf::RenderWindow& window, const sf::Font& font, sf::Vector2f position) const;
};