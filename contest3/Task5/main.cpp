#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

struct Point {
    long long x, y;
    
    Point() = default;
    Point(long long x, long long y) : x(x), y(y) {}
    
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    
    friend std::istream& operator>>(std::istream& input, Point& point);
    friend std::ostream& operator<<(std::ostream& output, const Point& segment);
};

class Segment {
private:
    Point begin, end;
    
public:
    
    Segment() = default;
    Segment(const Point& begin, const Point& end) : begin(begin), end(end) {}
    
    long long getY(long long x) const;
    long long getMin() const;
    long long getMax() const;
    
    friend std::istream& operator>>(std::istream& input, Segment& segment);
    friend std::ostream& operator<<(std::ostream& output, const Segment& segment);
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

std::pair<Segment, Segment> findAnyIntersection(const std::vector<Segment>& segments);

int main() {
    size_t n;
    std::cin >> n;
    std::vector<Segment> segments(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> segments[i];
    }
    auto answer = findAnyIntersection(segments);
    std::cout << answer.first << "\n" << answer.second << std::endl;
}

long long Segment::getY(long long x) const {
    if (begin == end)
        return begin.y;
    return begin.y + (end.y - begin.y) * (x - begin.x) / (end.x - begin.x);
}

long long Segment::getMin() const {
    return std::min(begin.x, end.x);
}

long long Segment::getMax() const {
    return std::max(begin.x, end.x);
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

std::ostream& operator<<(std::ostream& output, const Segment& segment) {
    std::cout << segment.begin << " " << segment.end << std::endl;
    return output;
}

std::istream& operator>>(std::istream& input, Point& point) {
    std::cin >> point.x >> point.y;
    return input;
}

std::ostream& operator<<(std::ostream& output, const Point& point) {
    std::cout << point.x << " " << point.y;
    return output;
}

std::pair<Segment, Segment> findAnyIntersection(const std::vector<Segment>& segments) {
    std::set<Segment> currentSegments;
    std::vector<Event> events;
    for (size_t i = 0; i < segments.size(); ++i) {
        events.emplace_back(segments[i].getMin(), EventType::ADD, i);
        events.emplace_back(segments[i].getMax(), EventType::REMOVE, i);
    }
    std::sort(events.begin(), events.end());
    
    // TODO: finish finding
    return std::make_pair(Segment(), Segment());
}
