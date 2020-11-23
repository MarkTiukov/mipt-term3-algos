#include "Point.hpp"

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y && z == other.z;
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}

Point& Point::operator-=(const Point& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Point operator-(const Point& one, const Point& other) {
    Point result = one;
    result -= other;
    return result;
}

std::istream& operator>>(std::istream& input, Point& point) {
    input >> point.x >> point.y >> point.z;
    return input;
}
