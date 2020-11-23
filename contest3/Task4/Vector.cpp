#include "Vector.hpp"

bool Vector::operator==(const Vector& another) const {
    return this->x == another.x && this->y == another.y;
}

bool Vector::operator!=(const Vector& another) const {
    return !(*this == another);
}

long long Vector::crossProduct(const Vector& v1, const Vector& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

bool Vector::areCollinear(const Vector &v1, const Vector &v2) {
    return Vector::crossProduct(v1, v2) == 0;
}
