#include <iostream>
#include <vector>

struct Point {
    long long x, y;
    
    Point() = default;
    Point(long long x, long long y) : x(x), y(y) {}
};

struct Vector {
    Point begin, end;
    Point radius;
    
    Vector() = default;
};

class Polygon {
private:
    std::vector<Point> points;
public:
    Polygon() = default;
    Polygon(std::vector<Point> points): points(points) {}
    
    Polygon& operator=(const Polygon& other) = default;
    Polygon& operator+=(const Point& other);
    Polygon operator+(const Point& other);

    void addPoint(const Point& p);
    double getSquare();
};

std::istream& operator>>(std::istream& in, Polygon& polygon);

int main() {
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


std::istream& operator>>(std::istream& in, Polygon& polygon) {
    size_t number = 0;
    std::cin >> number;
    for (int i = 0; i < number; ++i) {
        Point newPoint;
        std::cin >> newPoint.x >> newPoint.y;
        polygon.addPoint(newPoint);
    }
    return in;
}

void Polygon::addPoint(const Point& p) {
    this->points.push_back(p);
}

Polygon& Polygon::operator+=(const Point& other) {
    // TODO: add realization of Minkowski addition
    return *this;
}

Polygon Polygon::operator+(const Point& other) {
    Polygon result = *this;
    result += other;
    return result;
}

double Polygon::getSquare() {
    double result = 0;
    return result;
}

