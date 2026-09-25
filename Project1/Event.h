#pragma once
#include <string>

enum class EventType {
    SPEED_UP,           
    DOUBLE_SCORE,
    CLEAR_BOTTOM_ROW
};

struct Event {
    EventType type;
    float triggerTime;
    float duration;
    std::string description;

    Event(EventType t, float trigger, float dur, const std::string& desc)
        : type(t), triggerTime(trigger), duration(dur), description(desc) {}
};