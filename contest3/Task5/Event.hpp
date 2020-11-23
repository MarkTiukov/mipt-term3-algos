#ifndef Event_hpp
#define Event_hpp

#include <iostream>

enum EventType { ADD = 1, REMOVE = -1};

class Event {
private:
    long long x;
    EventType type;
    size_t id;
    
public:
    Event() = delete;
    Event(long long x, EventType type, size_t id) : x(x), type(type), id(id) {}
    
    size_t getID() const { return id; }
    EventType getType() const { return type; }
    
    bool operator<(const Event& other) const;
};
#endif 
