#include "Point.hpp"

bool Point::operator==(const Point& other) const {
    return this->x == other.x && this->y == other.y;
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}

std::istream& operator>>(std::istream& input, Point& point) {
    std::cin >> point.x >> point.y;
    return input;
}

std::ostream& operator<<(std::ostream& output, const Point& point) {
    std::cout << point.x << " " << point.y;
    return output;
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
