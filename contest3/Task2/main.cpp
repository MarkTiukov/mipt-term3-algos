#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <deque>
#include <iomanip>

const double fPRECISION = 1e-9;

struct Point {
    double x, y;
    
    Point() = default;
    Point(double x, double y) : x(x), y(y) {}
};

extern std::ostream& operator<<(std::ostream& outputStream, const Point& point);

bool operator<(const Point& one, const Point& another);
bool operator>(const Point& one, const Point& another);
bool operator==(const Point& one, const Point& another);
bool operator!=(const Point& one, const Point& another);

void readData(std::vector<Point> &points);

double getLength(const Point& a, const Point& b);
bool checkLeftRotation(const Point& a, const Point& b,const Point& c);
void printMinFence(const std::vector<Point> &points);

int main() {
    std::vector<Point> points;
    readData(points);
    printMinFence(points);
}

void readData(std::vector<Point> &points) {
    size_t n;
    std::cin >> n;
    points = std::vector<Point>(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> points[i].x >> points[i].y;
    }
    std::sort(points.begin(), points.end());
}

bool operator==(const Point& one, const Point &other) {
    return std::abs(one.x - other.x) < fPRECISION && std::abs(one.y - other.y) < fPRECISION;
}

bool operator>(const Point& one, const Point &other) {
    return !(one < other || one == other);
}

bool operator!=(const Point& one, const Point &other) {
    return !(one == other);
}

bool operator<(const Point& one, const Point& another) {
    bool result = one.y < another.y;
    if (std::abs(one.y - another.y) < fPRECISION)
        result = one.x < another.x;
    return result;
}

void printMinFence(const std::vector<Point> &points) {
    std::deque<Point> fence = std::deque<Point>({points[0]});
    size_t secondElementPosition = 1;
    while (secondElementPosition < points.size() && points[secondElementPosition] == points[0])
        ++secondElementPosition;
    fence.push_back(points[secondElementPosition++]);
    for (size_t i = secondElementPosition; i < points.size(); ++i) {
        while (!checkLeftRotation(fence[fence.size() - 2], fence.back(), points[i])) {
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
    std::cout << std::setprecision(10) << fenceLength << std::endl;
}

extern std::ostream& operator<<(std::ostream& outputStream, const Point& point) {
    outputStream << point.x << " " << point.y;
    return outputStream;
}

bool checkLeftRotation(const Point& a, const Point& b,const Point& c) {
    Point vector1(b.x - a.x, b.y - a.y);
    Point vector2(c.x - b.x, c.y - b.y);
    return vector1.x * vector2.y - vector1.y * vector2.x >= 0 - fPRECISION;
}

double getLength(const Point& a, const Point& b) {
    return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}
