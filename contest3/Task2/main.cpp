#include <iostream>
#include <iomanip>
#include "MyVector.h"

const size_t fOUTPUT_PRECISION = 10;

extern std::ostream& operator<<(std::ostream& outputStream, const Point& point);

Point begin;

bool cmp(const Point &a, const Point &b);

void readAndProcessData(std::vector<Point> &points);
double getLength(const Point& a, const Point& b);
bool checkLeftRotation(const Point& a, const Point& b,const Point& c);
template<typename T>
size_t getPrelastElementIndex(const std::deque<T>& container) {
    return container.size() - 2;
}
void printMinFence(const std::vector<Point> &points);


int main() {
    std::vector<Point> points;
    readAndProcessData(points);
    printMinFence(points);
}

void readAndProcessData(std::vector<Point> &points) {
    size_t n;
    std::cin >> n;
    points = std::vector<Point>(n);
    size_t min_position = 0;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> points[i].x >> points[i].y;
        if (points[i] < points[min_position])
            min_position = i;
    }
    std::swap(points[0], points[min_position]);
    begin = points[0];
    std::sort(++points.begin(), points.end(), cmp);
}

void printMinFence(const std::vector<Point> &points) {
    std::deque<Point> fence = std::deque<Point>({points[0]});
    size_t secondElementPosition = 1;
    while (secondElementPosition < points.size() && points[secondElementPosition] == points[0])
        ++secondElementPosition;
    fence.push_back(points[secondElementPosition++]);
    for (size_t i = secondElementPosition; i < points.size(); ++i) {
        if (fence.back() == points[i])
            continue;
        while (!checkLeftRotation(fence[getPrelastElementIndex<Point>(fence)], fence.back(), points[i])) {
            fence.pop_back();
        }
        fence.push_back(points[i]);
    }
    double fenceLength = getLength(fence.back(), fence.front());
    while (fence.size() > 1) {
        Point topPoint = fence.back();
        fence.pop_back();
        fenceLength += getLength(fence.back(), topPoint);
    }
    std::cout << std::setprecision(fOUTPUT_PRECISION) << fenceLength << std::endl;

    
}


bool checkLeftRotation(const Point& a, const Point& b,const Point& c) {
    auto product = Vector::zCoordinateOfCrossProduct(Vector(b - a), Vector(c - a));
    return product >= 0;
}

double getLength(const Point& a, const Point& b) {
    return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

bool cmp(const Point &a, const Point &b) {
    Vector v1(a - begin);
    Vector v2(b - begin);
    long long product = Vector::zCoordinateOfCrossProduct(v1, v2);
    if (product == 0) {
        if (v1 == Vector(0, 0))
            return true;
        if (v2 == Vector(0, 0))
            return false;
        return v1.getX() * v1.getX() + v1.getY() * v1.getY() > v2.getX() * v2.getX() + v2.getY() * v2.getY() ;
    }
    return product > 0;
}
