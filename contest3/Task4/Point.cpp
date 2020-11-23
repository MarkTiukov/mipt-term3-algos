#include "Point.hpp"

Point& Point::operator-=(const Point& other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

Point Point::operator-(const Point& other) const {
    Point result = *this;
    result -= other;
    return result;
}

Point& Point::operator+=(const Point& other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
}

Point Point::operator+(const Point& other) const {
    Point result = *this;
    result += other;
    return result;
}
