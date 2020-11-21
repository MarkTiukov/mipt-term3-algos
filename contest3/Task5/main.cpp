#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

const static double PRECISION = 1e-6;

struct Point {
    long long x, y;
    
    Point() = default;
    Point(long long x, long long y) : x(x), y(y) {}
    
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    
    Point& operator-=(const Point& other);
    friend Point operator-(const Point& one, const Point& other);
    
    friend std::istream& operator>>(std::istream& input, Point& point);
    friend std::ostream& operator<<(std::ostream& output, const Point& segment);
};

class Vector {
private:
    Point begin;
    Point end;
    long long x, y;
public:
    
    Vector() = default;
    Vector(const Point& begin, const Point& end) : begin(begin), end(end),
                                                   x(end.x - begin.x),
                                                   y(end.y - begin.y) {}
    Vector(const Point& radiusVector) : end(radiusVector),
                                        x(radiusVector.x),
                                        y(radiusVector.y) {}
    Vector(long long x, long long y) : x(x), y(y), end(Point(x, y)) {}
    
    long long getX() const { return x; }
    long long getY() const { return y; }
    Point getBegin() const { return begin; }
    Point getEnd() const { return end; }
    
    bool operator==(const Vector& another) const;
    bool operator!=(const Vector& another) const;
    
    static long long zCoordinateOfCrossProduct(const Vector& v1, const Vector& v2);
    static long long zCoordinateOfCrossProduct(const Point& begin, const Point& end1, const Point& end2);

};

class Segment {
private:
    Point begin, end;
public:
    const size_t id;
    
    Segment(size_t id) : id(id) {}
    Segment(const Point& begin, const Point& end, size_t id) : begin(begin), end(end), id(id) {}
    
    double getY(long long x) const;
    long long getMin() const;
    long long getMax() const;
    bool hasIntersectionByX(const Segment& other) const;
    bool hasIntersectionByY(const Segment& other) const;
    bool hasIntersectionWith(const Segment& other) const;
    
    bool operator<(const Segment& other) const;
    
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
    
    size_t getID() const { return id; }
    EventType getType() const { return type; }
    
    bool operator<(const Event& other) const;
};

class IntersectionFinder {
private:
    std::set<Segment> currentSegments;
    
    std::set<Segment>::iterator prev(std::set<Segment>::iterator it);
    std::set<Segment>::iterator next(std::set<Segment>::iterator it);
public:
    IntersectionFinder() = default;
    std::pair<Segment, Segment> findAnyIntersection(const std::vector<Segment>& segments, bool& hasFound);
};


int main() {
    size_t n;
    std::cin >> n;
    std::vector<Segment> segments;
    for (size_t i = 0; i < n; ++i) {
        segments.emplace_back(i);
        std::cin >> segments[i];
    }
    IntersectionFinder solution;
    bool intersect;
    auto answer = solution.findAnyIntersection(segments, intersect);
    if (intersect)
        std::cout << "YES\n" << answer.first.id + 1 << " " << answer.second.id + 1 << std::endl;
    else
        std::cout << "NO" << std::endl;
}

double Segment::getY(long long x) const {
    if (begin.x == end.x)
        return begin.y;
    double by = (double) begin.y;
    double bx = (double) begin.x;
    double ey = (double) end.y;
    double ex = (double) end.x;
    
    return by + (ey - by) * (x - bx) / (ex - bx);
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

std::pair<Segment, Segment> IntersectionFinder::findAnyIntersection(const std::vector<Segment>& segments, bool& hasFound) {
    hasFound = true;
    std::vector<std::set<Segment>::iterator> segmentsInSet(segments.size());
    std::vector<Event> events;
    for (size_t i = 0; i < segments.size(); ++i) {
        events.emplace_back(segments[i].getMin(), EventType::ADD, i);
        events.emplace_back(segments[i].getMax(), EventType::REMOVE, i);
    }
    std::sort(events.begin(), events.end());
    for (auto event : events) {
        size_t currentID = event.getID();
        if (event.getType() == EventType::ADD) {
            std::set<Segment>::iterator nextSegmentIterator = currentSegments.lower_bound(segments[currentID]);
            std::set<Segment>::iterator prevSegmentIterator = prev(nextSegmentIterator);
            if (nextSegmentIterator != currentSegments.end() && (*nextSegmentIterator).hasIntersectionWith(segments[currentID]))
                return std::make_pair(*nextSegmentIterator, segments[currentID]);
            if (prevSegmentIterator != currentSegments.end() && (*prevSegmentIterator).hasIntersectionWith(segments[currentID]))
                return std::make_pair(*prevSegmentIterator, segments[currentID]);
            segmentsInSet[currentID] = currentSegments.insert(nextSegmentIterator, segments[currentID]);
        } else if (event.getType() == EventType::REMOVE) {
            std::set<Segment>::iterator nextSegmentIterator = next(segmentsInSet[currentID]);
            std::set<Segment>::iterator prevSegmentIterator = prev(segmentsInSet[currentID]);
            if (nextSegmentIterator != currentSegments.end() && prevSegmentIterator != currentSegments.end() &&
                (*prevSegmentIterator).hasIntersectionWith(*nextSegmentIterator))
                return std::make_pair(*prevSegmentIterator, *nextSegmentIterator);
        }
    }
    hasFound = false;
    return std::make_pair(Segment(0), Segment(0));
}

std::set<Segment>::iterator IntersectionFinder::prev(std::set<Segment>::iterator it) {
    if (it != currentSegments.begin())
        return std::prev(it);
    return currentSegments.end();
}

std::set<Segment>::iterator IntersectionFinder::next(std::set<Segment>::iterator it) {
    return std::next(it);
}

bool Segment::operator<(const Segment& other) const {
    double x = std::max(std::min(begin.x, end.x), std::min(other.begin.x, other.end.x));
    return getY(x) < other.getY(x) - PRECISION;
}

bool Segment::hasIntersectionWith(const Segment &other) const {
    bool result = false;
    if (hasIntersectionByX(other) && hasIntersectionByY(other))
        if (Vector::zCoordinateOfCrossProduct(begin, end, other.begin) *
            Vector::zCoordinateOfCrossProduct(begin, end, other.end) <= 0 &&
            Vector::zCoordinateOfCrossProduct(other.begin, other.end, begin) *
            Vector::zCoordinateOfCrossProduct(other.begin, other.end, end) <= 0)
            result = true;
    return result;
}

bool Segment::hasIntersectionByX(const Segment &other) const {
    long long leftEnd1 = std::min(begin.x, end.x);
    long long rightEnd1 = std::max(begin.x, end.x);
    long long leftEnd2 = std::min(other.begin.x, other.end.x);
    long long rightEnd2 = std::max(other.begin.x, other.end.x);
    return std::max(leftEnd1, leftEnd2) <= std::min(rightEnd1, rightEnd2);
}

bool Segment::hasIntersectionByY(const Segment &other) const {
    long long lowerEnd1 = std::min(begin.y, end.y);
    long long upperEnd1 = std::max(begin.y, end.y);
    long long lowerEnd2 = std::min(other.begin.y, other.end.y);
    long long upperEnd2 = std::max(other.begin.y, other.end.y);
    return std::max(lowerEnd1, lowerEnd2) <= std::min(upperEnd1, upperEnd2);
}

long long Vector::zCoordinateOfCrossProduct(const Vector& v1, const Vector& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

long long Vector::zCoordinateOfCrossProduct(const Point &begin, const Point &end1, const Point &end2) {
    return zCoordinateOfCrossProduct(Vector(end1 - begin), Vector(end2 - begin));
}


Point& Point::operator-=(const Point& other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

Point operator-(const Point& one, const Point& other) {
    Point result = one;
    result -= other;
    return result;
}
