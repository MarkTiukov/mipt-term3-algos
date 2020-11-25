#include "Point.h"

Point& Point::operator-=(const Point& another) {
    this->x -= another.x;
    this->y -= another.y;
    return *this;
}

Point operator-(const Point& one, const Point& another) {
    Point result = one;
    result -= another;
    return result;
}

bool operator==(const Point& one, const Point &other) {
    return one.x == other.x && one.y == other.y;
}

bool operator>(const Point& one, const Point &other) {
    return !(one < other || one == other);
}

bool operator!=(const Point& one, const Point &other) {
    return !(one == other);
}

bool operator<(const Point& one, const Point& another) {
    bool result = one.y < another.y;
    if (one.y == another.y)
        result = one.x < another.x;
    return result;
}

std::ostream& operator<<(std::ostream& outputStream, const Point& point) {
    outputStream << point.x << " " << point.y;
    return outputStream;
}
