#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <cmath>

struct Point {
    double x, y, z;
    
    Point() = default;
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
    
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    
    Point& operator-=(const Point& other);
    friend Point operator-(const Point& one, const Point& other);
    
};

class Vector {
private:
    Point begin;
    Point end;
    double x, y, z;
public:
    
    Vector() = default;
    Vector(const Point& begin, const Point& end) : begin(begin), end(end),
                                                   x(end.x - begin.x),
                                                   y(end.y - begin.y),
                                                   z(end.z - begin.z){}
    Vector(double x, double y, double z) : x(x), y(y), z(z), end(Point(x, y, z)) {}
    
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    Point getBegin() const { return begin; }
    Point getEnd() const { return end; }
    
    friend Vector operator*(const Vector& one, const Vector& other);
};

class Face {
private:
    size_t first;
    size_t second;
    size_t third;
public:
    Face() = default;
    Face(size_t first, size_t second, size_t third) : first(first), second(second), third(third) {}
    
    size_t getFirst() const { return first; }
    size_t getSecond() const { return second; }
    size_t getThird() const { return third; }
    
    void makeRightOrder();
    void swapFirstAndSecond();
    
    bool operator<(const Face& other) const;
    friend std::ostream& operator<<(std::ostream& out, const Face& face);
};

class ConvexHull {
public:
    struct HullPoint: Point {
    private:
        static int32_t totalNumber;
        void rotateAroundThirdDimension(double& x, double& y, const double& angle);
    public:
        int32_t id = totalNumber++;
        HullPoint* prev = nullptr;
        HullPoint* next = nullptr;
        
        HullPoint() = default;
        HullPoint(const Point& point) : Point(point) {}
        
        void resetTotalNumber() { totalNumber = 0; }
        void rotate(double angle);
        bool holdMovie();
        
        friend bool operator<(const HullPoint& one, const HullPoint& other);
    };
    
private:
    std::vector<HullPoint> points;
    std::vector<Face> hull;
    
    double time(const HullPoint* a, const HullPoint* b, const HullPoint* c);
    bool antiClockwise(const HullPoint* a, const HullPoint* b, const HullPoint* c);
    void supportingRib(HullPoint*& u, HullPoint*& v);
    std::vector<HullPoint*> mergeMovies(int64_t leftBorder, int64_t rightBorder);
    void buildConvexHull();
    std::vector<Face> buildConvexHull(bool toSwap);
    
public:
    const double INF = 1e9;

    ConvexHull(const std::vector<Point>& points);

    std::vector<Face> getHull() const { return hull; }
    size_t getHullSize() const { return hull.size(); }

    friend std::ostream& operator<<(std::ostream& out, const ConvexHull& convexHull);
    friend bool operator<(const HullPoint& one, const HullPoint& other);

};

int32_t ConvexHull::HullPoint::totalNumber = 0;

int main() {
    size_t testsNumber;
    std::cin >> testsNumber;
    for (int k = 0; k < testsNumber; ++k) {
        size_t n;
        std::cin >> n;
        std::vector<Point> points;
        for (size_t i = 0; i < n; ++i) {
            Point p;
            std::cin >> p.x >> p.y >> p.z;
            points.push_back(p);
        }
        ConvexHull convex_hull(points);
        std::cout << convex_hull << "\n";
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& output, const ConvexHull& convexHull) {
    output << convexHull.getHullSize() << "\n";
    for (auto face : convexHull.getHull()) {
        output << "3 " << face << "\n";
    }
    return output;
}

double ConvexHull::time(const HullPoint* a, const HullPoint* b, const HullPoint* c) {
    if (a == nullptr || b == nullptr || c == nullptr)
        return INF;
    auto product = Vector(*a, *b) * Vector(*b, *c);
    return -product.getY() / product.getZ();
}

bool ConvexHull::antiClockwise(const HullPoint* a, const HullPoint* b, const HullPoint* c) {
    if (a == nullptr || b == nullptr || c == nullptr)
        return true;
    return (Vector(*a, *b) * Vector(*b, *c)).getZ() > 0;
}

void ConvexHull::supportingRib(HullPoint*& u, HullPoint*& v) {
    while (true) {
        if (!antiClockwise(u->prev, u, v))
            u = u->prev;
        else if (!antiClockwise(u, v, v->next))
            v = v->next;
        else
            break;
    }
}

std::vector<ConvexHull::HullPoint*> ConvexHull::mergeMovies(int64_t leftBorder, int64_t rightBorder) {
    if (rightBorder - leftBorder <= 1)
        return std::vector<HullPoint*>();
    const int64_t middle = (leftBorder + rightBorder) / 2;
    const std::vector<HullPoint*> leftPart = mergeMovies(leftBorder, middle);
    const std::vector<HullPoint*> rightPart = mergeMovies(middle, rightBorder);
    std::vector<HullPoint*> movies;
    auto u = &points[middle - 1];
    auto v = &points[middle];
    supportingRib(u, v);
    int64_t currentLeft = 0;
    int64_t currentRight = 0;
    for (auto currentTime = -INF; true;) {
        HullPoint* left = nullptr;
        HullPoint* right = nullptr;
        std::vector<double> times(6, INF);
        if (currentLeft < leftPart.size()) {
            left = leftPart[currentLeft];
            times[0] = time(left->prev, left, left->next);
        }
        if (currentRight < rightPart.size()) {
            right = rightPart[currentRight];
            times[1] = time(right->prev, right, right->next);
        }
        times[2] = time(u->prev, u, v);
        times[3] = time(u, u->next, v);
        times[4] = time(u, v, v->next);
        times[5] = time(u, v->prev, v);
        int64_t minTimePosition = 0;
        auto minTime = times[minTimePosition];
        for (int64_t i = 1; i < times.size(); ++i) {
            if (currentTime < times[i] && times[i] < minTime) {
                minTimePosition = i;
                minTime = times[minTimePosition];
            }
        }
        currentTime = minTime;
        if (minTime >= INF)
            break;
        switch (minTimePosition) {
            case 0:
                if (left->x < u->x)
                    movies.push_back(left);
                left->holdMovie();
                ++currentLeft;
                break;
            case 1:
                if (right->x > v->x)
                    movies.push_back(right);
                right->holdMovie();
                ++currentRight;
                break;
            case 2:
                movies.push_back(u);
                u = u->prev;
                break;
            case 3:
                movies.push_back(u = u->next);
                break;
            case 4:
                movies.push_back(v);
                v = v->next;
                break;
            case 5:
                movies.push_back(v = v->prev);
                break;
            default:
                break;
        }
    }
    u->next = v;
    v->prev = u;
    for (auto it = movies.rbegin(); it != movies.rend(); ++it) {
        auto& current = *it;
        if (u->x < current->x && current->x < v->x) {
            u->next = v->prev = current;
            current->prev = u;
            current->next = v;
            if (current->x <= points[middle - 1].x) {
                u = current;
            } else {
                v = current;
            }
        } else {
            current->holdMovie();
            if (current == u)
                u = u->prev;
            if (current == v)
                v = v->next;
        }
    }
    return movies;
}

std::vector<Face> ConvexHull::buildConvexHull(bool toSwap) {
    std::vector<Face> hull;
    std::vector<HullPoint*> movies = mergeMovies(0, points.size());
    for (HullPoint* HullPoint : movies) {
        Face face(HullPoint->prev->id, HullPoint->id, HullPoint->next->id);
        if (HullPoint->holdMovie() == !toSwap) {
            face.swapFirstAndSecond();
        }
        hull.push_back(face);
    }
    return hull;
}

void ConvexHull::buildConvexHull() {
    std::sort(points.begin(), points.end());
    hull = buildConvexHull(true);
    for (HullPoint& p : points) {
        p.prev = nullptr;
        p.next = nullptr;
        p.z = -p.z;
    }
    auto second_hull = buildConvexHull(false);
    hull.insert(hull.end(), second_hull.begin(), second_hull.end());
    for (Face& triple : hull) {
        triple.makeRightOrder();
    }
    std::sort(hull.begin(), hull.end());
}

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

Vector operator*(const Vector& one, const Vector& other) {
    return Vector(one.y * other.z - one.z * other.y, -(one.x * other.z - one.z * other.x), one.x * other.y - one.y * other.x);
}

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

void ConvexHull::HullPoint::rotateAroundThirdDimension(double& x, double& y, const double& angle) {
    const double cos = std::cos(angle);
    const double sin = std::sin(angle);
    double newX, newY;
    newX = x * cos + y * sin;
    newY = -x * sin + y * cos;
    x = newX;
    y = newY;
}

void ConvexHull::HullPoint::rotate(double angle) {
    rotateAroundThirdDimension(y, z, angle);
    rotateAroundThirdDimension(x, z, angle);
    rotateAroundThirdDimension(x, y, angle);
}

bool ConvexHull::HullPoint::holdMovie() {
    if (prev->next != this) {
        prev->next = next->prev = this;
        return true;
    }
    prev->next = next;
    next->prev = prev;
    return false;
}

ConvexHull::ConvexHull(const std::vector<Point>& points) {
    constexpr double ANGLE = 1e-2;
    for (auto& point : points) {
        this->points.push_back(point);
        this->points.back().rotate(ANGLE);
    }
    buildConvexHull();
    this->points.back().resetTotalNumber();
}

bool operator<(const ConvexHull::HullPoint& one, const ConvexHull::HullPoint& other) {
    return one.x < other.x;
}
