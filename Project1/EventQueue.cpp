#include "EventQueue.h"

EventQueue::EventQueue() : front(nullptr), count(0) {}

EventQueue::~EventQueue() {
    clear();
}

void EventQueue::clear() {
    while (front != nullptr) {
        EventNode* temp = front;
        front = front->next;
        delete temp;
    }
    count = 0;
}

void EventQueue::enqueue(const Event& event) {
    EventNode* newNode = new EventNode(event);

    // Caso 1: La cola esta vacia o el nuevo evento debe ir al frente
    if (front == nullptr || event.triggerTime < front->event.triggerTime) {
        newNode->next = front;
        front = newNode;
    }
    // Caso 2: Buscar la posicion correcta en la lista para mantener el orden ascendente
    else {
        EventNode* current = front;
        while (current->next != nullptr && current->next->event.triggerTime <= event.triggerTime) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    count++;
}

Event EventQueue::dequeue() {
    if (isEmpty()) {
        return Event(EventType::SPEED_UP, 0.f, 0.f, "");
    }

    EventNode* temp = front;
    Event poppedEvent = temp->event;

    front = front->next;
    delete temp;
    count--;

    return poppedEvent;
}

std::optional<Event> EventQueue::peek() const {
    if (isEmpty()) return std::nullopt;
    return front->event;
}

bool EventQueue::isEmpty() const {
    return front == nullptr;
}

void EventQueue::drawUI(sf::RenderWindow& window, const sf::Font& font, sf::Vector2f position) const {
    sf::Text titleText(font, "PROXIMOS EVENTOS", 14);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition(position);
    window.draw(titleText);

    EventNode* current = front;
    float startY = position.y + 22.f;
    int drawn = 0;

    while (current && drawn < 2) {
        std::string info = "- " + current->event.description + " (" + std::to_string(static_cast<int>(current->event.triggerTime)) + "s)";
        sf::Text eventText(font, info, 12);
        eventText.setFillColor(sf::Color::White);
        eventText.setPosition({ position.x, startY });
        window.draw(eventText);

        startY += 18.f;
        current = current->next;
        drawn++;
    }
}

std::vector<Event> EventQueue::getQueueState() const {
    std::vector<Event> events;
    EventNode* current = front;
    while (current) {
        events.push_back(current->event);
        current = current->next;
    }
    return events;
}

void EventQueue::setQueueState(const std::vector<Event>& events) {
    clear(); // Libera los nodos actuales
    for (const auto& ev : events) {
        enqueue(ev); // Reinserta los eventos en orden
    }
}