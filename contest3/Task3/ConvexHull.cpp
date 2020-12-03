#include "ConvexHull.hpp"

#include <cmath>

size_t ConvexHull::HullPoint::totalNumber = 0;

std::ostream& operator<<(std::ostream& output, const ConvexHull& convexHull) {
    output << convexHull.getHullSize() << "\n";
    for (auto face : convexHull.getHull()) {
        output << face.NUMBER_OF_VERTEXES << " " << face << "\n";
    }
    return output;
}

double ConvexHull::time(const HullPoint* a, const HullPoint* b, const HullPoint* c) {
    if (a == nullptr || b == nullptr || c == nullptr)
        return INF;
    auto product = Vector(*a, *b) * Vector(*b, *c);
    return -product.getY() / product.getZ();
}

bool ConvexHull::isLeftRotation(const HullPoint* a, const HullPoint* b, const HullPoint* c) {
    if (a == nullptr || b == nullptr || c == nullptr)
        return true;
    return (Vector(*a, *b) * Vector(*b, *c)).getZ() > 0;
}

void ConvexHull::findSupportingRib(HullPoint*& u, HullPoint*& v) {
    while (true) {
        if (!isLeftRotation(u->prev, u, v))
            u = u->prev;
        else if (!isLeftRotation(u, v, v->next))
            v = v->next;
        else
            break;
    }
}

std::vector<ConvexHull::HullPoint*> ConvexHull::merge(size_t leftBorder, size_t rightBorder) {
    if (rightBorder - leftBorder <= 1)
        return std::vector<HullPoint*>();
    const size_t middle = (leftBorder + rightBorder) / 2;
    const std::vector<HullPoint*> leftPart = merge(leftBorder, middle);
    const std::vector<HullPoint*> rightPart = merge(middle, rightBorder);
    std::vector<HullPoint*> movies;
    auto u = &points[middle - 1];
    auto v = &points[middle];
    findSupportingRib(u, v);
    size_t currentLeft = 0;
    size_t currentRight = 0;
    for (auto currentTime = -INF; true;) {
        HullPoint* left = nullptr;
        HullPoint* right = nullptr;
        std::vector<double> times(NUMBER_OF_POINTS_TO_WATCH, INF);
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
        size_t minTimePosition = 0;
        auto minTime = times[minTimePosition];
        for (size_t i = 1; i < times.size(); ++i) {
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
    std::vector<HullPoint*> movies = merge(0, points.size());
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
    for (auto point: points)
        originalPoints.push_back(point);
    buildConvexHull();
    this->points.back().resetTotalNumber();
}

double ConvexHull::distanceToInsidePoint(const Point &point) const {
    double minDistance = INF;
    for (Face face : hull) {
        double x1 = originalPoints[face.getFirst()].x, y1 = originalPoints[face.getFirst()].y, z1 = originalPoints[face.getFirst()].z;
        double x2 = originalPoints[face.getSecond()].x, y2 = originalPoints[face.getSecond()].y, z2 = originalPoints[face.getSecond()].z;
        double x3 = originalPoints[face.getThird()].x, y3 = originalPoints[face.getThird()].y, z3 = originalPoints[face.getThird()].z;
        double A = y1 * z2 - y1 * z3 - y2 * z1 + y2 * z3 + y3 * z1 - y3 * z2;
        double B = -x1 * z2 + x1 * z3 + x2 * z1 - x2 * z3 - x3 * z1 + x3 * z2;
        double C = x1 * y2 - x1 * y3 - x2 * y1 + x2 * y3 + x3 * y1 - x3 * y2;
        double D = -x1 * y2 * z3 + x1 * y3 * z2 + x2 * y1 * z3 - x2 * y3 * z1 - x3 * y1 * z2 + x3 * y2 * z1;
        double denominator = std::sqrt(A * A + B * B + C * C);
        double currentDistance = std::abs(A * point.x + B * point.y + C * point.z + D) / denominator;
        if (currentDistance < minDistance)
            minDistance = currentDistance;
    }
    return minDistance;
}

bool operator<(const ConvexHull::HullPoint& one, const ConvexHull::HullPoint& other) {
    return one.x < other.x;
}
