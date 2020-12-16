#ifndef Polygon_hpp
#define Polygon_hpp

#include <vector>

#include "Point.hpp"
#include "Vector.hpp"

class Polygon {
private:
    std::vector<Point> points;
    
    static bool isLowerAndToTheLeft(const Point& p1, const Point& p2);
    static bool compareByAngle(const Point& p1, const Point& p2, const Point& begin);
    static bool compareByAngle(const Vector& v1, const Vector& v2);
    
    
    struct LowerAndToTheLeftComparator : std::binary_function<Point, Point, bool> {
        Polygon *polygon;
        
        LowerAndToTheLeftComparator(Polygon *p) : polygon(p) {}
        bool operator()(const Point& p1, const Point& p2) {
            return polygon->isLowerAndToTheLeft(p1, p2);
        }
    };
    
    struct AngleComparator : std::binary_function<Point, Point, bool> {
        Polygon *polygon;
        Point begin;
        AngleComparator(Polygon *p, const Point& center) : polygon(p), begin(center) {}
        bool operator()(const Point& p1, const Point& p2) {
            return polygon->compareByAngle(p1, p2, this->begin);
        }
    };
    
public:
    Polygon() = default;
    Polygon(std::vector<Point> points) : points(points) {}
    
    Polygon& operator=(const Polygon& other) = default;
    Polygon& operator+=(const Polygon& other);
    Polygon operator+(const Polygon& other) const;
    
    Polygon& operator+=(const Point& newPoint);
    Polygon operator+(const Point& newPoint) const;
    
    void sort();
    long double getSquare() const;
    
    friend std::istream& operator>>(std::istream& in, Polygon& polygon);
    
};

#endif
