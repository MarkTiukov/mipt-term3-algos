#include "Event.hpp"

bool Event::operator<(const Event& other) const {
    if (x == other.x)
        return type > other.type;
    return x < other.x;
}
