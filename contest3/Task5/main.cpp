#include <iostream>
#include <algorithm>
#include <vector>

struct Point {
    long long x, y;
    
    Point() = default;
    Point(long long x, long long y) : x(x), y(y) {}
    
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    
    friend std::istream& operator>>(std::istream& input, Point& point);
};

class Segment {
private:
    Point begin, end;

public:
    
    Segment() = default;
    Segment(const Point& begin, const Point& end) : begin(begin), end(end) {}
    
    long long getY(long long x) const;
    
    friend std::istream& operator>>(std::istream& input, Segment& segment);
};

enum EventType { ADD = 1, REMOVE = -1};

class Event {
private:
    long long x;
    EventType type;
    size_t id;
    
public:
    Event() = delete;
    Event(long long x, EventType type, size_t id) : x(x), type(type), id(id) {}
    
    bool operator<(const Event& other) const;
};

int main() {
    size_t n;
    std::cin >> n;
    std::vector<Segment> segments(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> segments[i];
    }
}

long long Segment::getY(long long x) const {
    if (begin == end)
        return begin.y;
    return begin.y + (end.y - begin.y) * (x - begin.x) / (end.x - begin.x);
}

bool Point::operator==(const Point& other) const {
    return this->x == other.x && this->y == other.y;
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}

bool Event::operator<(const Event& other) const {
    if (x == other.x)
        return type > other.type;
    return x < other.x;
}

std::istream& operator>>(std::istream& input, Segment& segment) {
    std::cin >> segment.begin >> segment.end;
    return input;
}

std::istream& operator>>(std::istream& input, Point& point) {
    std::cin >> point.x >> point.y;
    return input;
}
