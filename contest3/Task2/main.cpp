#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <deque>
#include <iomanip>
#include <climits>

struct Point {
    long long x, y;
    
    Point() = default;
    Point(long long x, long long y) : x(x), y(y) {}
    
};

extern std::ostream& operator<<(std::ostream& outputStream, const Point& point);

bool operator<(const Point& one, const Point& another);
bool operator>(const Point& one, const Point& another);
bool operator==(const Point& one, const Point& another);
bool operator!=(const Point& one, const Point& another);

Point begin;

bool cmp(const Point &a, const Point &b);
long long crossProduct(const Point& v1, const Point& v2);

void readData(std::vector<Point> &points);
double getLength(const Point& a, const Point& b);
bool checkOnePointAfterTwoOthers(const Point& a, const Point& b,const Point& c);
bool checkLeftRotation(const Point& a, const Point& b,const Point& c);
bool checkAreOnLine(const Point& a, const Point& b, const Point&c);
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
    size_t min_position = 0;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> points[i].x >> points[i].y;
        if (i == 0) {
            min_position = 0;
        }
        else {
            if (points[i] < points[min_position])
                min_position = i;
        }
    }
    std::swap(points[0], points[min_position]);
    begin = points[0];
    std::sort(++points.begin(), points.end(), cmp);
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

void printMinFence(const std::vector<Point> &points) {
    std::deque<Point> fence = std::deque<Point>({points[0]});
    size_t secondElementPosition = 1;
    while (secondElementPosition < points.size() && points[secondElementPosition] == points[0])
        ++secondElementPosition;
    if (secondElementPosition == points.size()) {
        std::cout << "0" << std::endl;;
    } else {
        fence.push_back(points[secondElementPosition++]);
        for (size_t i = secondElementPosition; i < points.size(); ++i) {
            if (fence.back() == points[i])
                continue;
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
    
}

extern std::ostream& operator<<(std::ostream& outputStream, const Point& point) {
    outputStream << point.x << " " << point.y;
    return outputStream;
}

bool checkOnePointAfterTwoOthers(const Point& a, const Point& b,const Point& c) {
    bool result = false;
    if (getLength(c, a) >= getLength(a, b) && getLength(c, b) <= getLength(c, a))
        result = true;
    return result;
}

bool checkLeftRotation(const Point& a, const Point& b,const Point& c) {
    auto product = crossProduct(Point(b.x - a.x, b.y - a.y), Point(c.x - b.x, c.y - b.y));
    return product >= 0;
}

bool checkAreOnLine(const Point& a, const Point& b, const Point& c) {
    return crossProduct(Point(b.x - a.x, b.y - a.y), Point(c.x - b.x, c.y - b.y)) == 0;
}

double getLength(const Point& a, const Point& b) {
    return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

bool cmp(const Point &a, const Point &b) {
    Point v1(a.x - begin.x, a.y - begin.y);
    Point v2(b.x - begin.x, b.y - begin.y);
    long long product = crossProduct(v1, v2);
    if (product == 0) {
        if (v1 == Point(0, 0))
            return true;
        if (v2 == Point(0, 0))
            return false;
        return v1.x * v1.x + v1.y * v1.y > v2.x * v2.x + v2.y * v2.y;
    }
    return product > 0;
}

long long crossProduct(const Point& v1, const Point& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}
