#include "Face.hpp"

bool Face::operator<(const Face& other) const {
    if (first == other.first) {
        if (second == other.second)
            return third < other.third;
        return second < other.second;
    }
    return first < other.first;
}

void Face::makeRightOrder() {
    auto oldFirst = getFirst();
    auto oldSecond = getSecond();
    auto oldThird = getThird();
    
    if (oldSecond < oldFirst && oldSecond < oldThird) {
        *this = Face(oldSecond, oldThird, oldFirst);
    } else if (oldThird < oldFirst && oldThird < oldSecond) {
        *this = Face(oldThird, oldFirst, oldSecond);
    }
}

void Face::swapFirstAndSecond() {
    std::swap(first, second);
}

std::ostream& operator<<(std::ostream& out, const Face& face) {
    return out << face.first << " " << face.second << " " << face.third;
}
