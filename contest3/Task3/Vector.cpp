#include "Vector.hpp"

Vector operator*(const Vector& one, const Vector& other) {
    return Vector(one.y * other.z - one.z * other.y, -(one.x * other.z - one.z * other.x), one.x * other.y - one.y * other.x);
}
