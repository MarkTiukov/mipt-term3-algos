#include <iostream>
#include <cmath>

const double fPRECISION = 1e-6;

struct Point {
    double x, y, z;
};

struct Segment {
    Point a, b;
    
    Segment(double x1, double y1, double z1, double x2, double y2, double z2) {
        a = Point();
        a.x = x1;
        a.y = y1;
        a.z = z1;
        b = Point();
        b.x = x2;
        b.y = y2;
        b.z = z2;
    }
};

double getHeightFromPointOnSegment(const Segment &segment, const Point &point);

bool checkIntersectionOnProjection(const Segment &segment1, const Segment &segment2);
bool checkIntersection(const Segment &segment1, const Segment &segment2);

double findMinDistanceBetweenTwoSegments(const Segment &segment1, const Segment &segment2);

int main() {
    Point a, b, c, d;
    std::cin >> a.x >> a.y >> a.z;
    std::cin >> b.x >> b.y >> b.z;
    std::cin >> c.x >> c.y >> c.z;
    std::cin >> d.x >> d.y >> d.z;
}

bool checkIntersectionOnProjection(const Segment &segment1, const Segment &segment2) {
    bool result = false;
    double det = (segment1.b.x-segment1.a.x) * (segment2.a.y - segment2.b.y) -
    (segment1.b.y - segment1.a.y) * (segment2.a.x - segment2.b.x);
    double det1 = (segment1.b.x-segment1.a.x) * (segment2.a.y - segment1.a.y) -
    (segment1.b.y - segment1.a.y) * (segment2.a.x - segment1.a.x);
    double det2 = (segment2.a.x-segment1.a.x) * (segment2.a.y - segment2.b.y) -
    (segment2.a.y - segment1.a.y) * (segment2.a.x - segment2.b.x);
    if (std::abs(det) < fPRECISION) {
        double part_x = det1 / det;
        double part_y = det2 / det;
        if (part_x >= 0 && part_y >=0 && part_x <= 1 && part_y <= 1) {
            result = true;
        }
    }
    return result;
}

double findMinDistanceBetweenTwoSegments(const Segment &segment1, const Segment &segment2) {
    if (checkIntersection(segment1, segment2))
        return 0.0;
    double result = getHeightFromPointOnSegment(segment1, segment2.a);
    double newHeight = getHeightFromPointOnSegment(segment1, segment2.b);
    if (newHeight != -1 && newHeight < result)
        result = newHeight;
    newHeight = getHeightFromPointOnSegment(segment2, segment1.a);
    if (newHeight != -1 && newHeight < result)
        result = newHeight;
    newHeight = getHeightFromPointOnSegment(segment2, segment1.b);
    if (newHeight != -1 && newHeight < result)
        result = newHeight;
    if (result == -1) {
        result = (segment1.a.x - segment2.a.x) * (segment1.a.x - segment2.a.x)
        + (segment1.a.y - segment2.a.y) * (segment1.a.y - segment2.a.y);
        newHeight = (segment1.b.x - segment2.b.x) * (segment1.b.x - segment2.b.x)
        + (segment1.b.y - segment2.b.y) * (segment1.b.y - segment2.b.y);
        if(newHeight < result)
            result = newHeight;
        newHeight = (segment1.b.x - segment2.a.x) * (segment1.b.x - segment2.a.x)
        + (segment1.b.y - segment2.a.y) * (segment1.b.y - segment2.a.y);
        if(newHeight < result)
            result = newHeight;
        newHeight = (segment1.a.x - segment2.b.x) * (segment1.a.x - segment2.b.x)
        + (segment1.a.y - segment2.b.y) * (segment1.a.y - segment2.b.y);
        if(newHeight < result)
            result = newHeight;
        result = std::sqrt(result);
    }
    return result;
}

double getHeightFromPointOnSegment(const Segment &segment, const Point &point) {
    return 0.0;
}

bool checkIntersection(const Segment &segment1, const Segment &segment2) {
    return checkIntersectionOnProjection(Segment(segment1.a.x, segment1.a.y, 0, segment1.b.x, segment1.b.y, 0),
                                                Segment(segment2.a.x, segment2.a.y, 0, segment2.b.x, segment2.b.y, 0)) &&
                    checkIntersectionOnProjection(Segment(segment1.a.x, 0, segment1.a.z, segment1.b.x, 0, segment1.b.z),
                                                  Segment(segment2.a.x, 0, segment2.a.z, segment2.b.x, 0, segment2.b.z)) &&
                    checkIntersectionOnProjection(Segment(0, segment1.a.y, segment1.a.z, 0, segment1.b.y, segment1.b.z),
                                                  Segment(0, segment2.a.y, segment2.a.z, 0, segment2.b.y, segment2.b.z));
}
