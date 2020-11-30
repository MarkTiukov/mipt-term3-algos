#ifndef ConvexHull_hpp
#define ConvexHull_hpp

#include <iostream>
#include <vector>

#include "Point.hpp"
#include "Vector.hpp"
#include "Face.hpp"


/// Creates a convex hull in space using Chan algorithm
/// https://drive.google.com/file/d/0B6oYnrxwl0UcTV9GamFfOHBHeXM/view?usp=sharing
class ConvexHull {
public:
    // It's better to use raw pointers beacuse of need to
    // store a vector of points allocated automatically,
    // bidirectional structure of HullPoint
    // and need to create temporary instances.
    // Similar situation:
    // https://stackoverflow.com/questions/24049155/set-shared-ptr-to-point-existing-object
    struct HullPoint: Point {
    private:
        static size_t totalNumber;
        void rotateAroundThirdDimension(double& x, double& y, const double& angle);
    public:
        size_t id = totalNumber++;
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
    const size_t NUMBER_OF_POINTS_TO_WATCH = 6;
    std::vector<HullPoint> points;
    std::vector<HullPoint> originalPoints;
    std::vector<Face> hull;
    
    double time(const HullPoint* a, const HullPoint* b, const HullPoint* c);
    bool isLeftRotation(const HullPoint* a, const HullPoint* b, const HullPoint* c);
    void findSupportingRib(HullPoint*& u, HullPoint*& v);
    std::vector<HullPoint*> merge(size_t leftBorder, size_t rightBorder);
    void buildConvexHull();
    std::vector<Face> buildConvexHull(bool toSwap);
    
public:
    const double INF = 1e9;
    
    ConvexHull(const std::vector<Point>& points);
    
    std::vector<Face> getHull() const { return hull; }
    size_t getHullSize() const { return hull.size(); }
    double distanceToInsidePoint(const Point& point) const;
    
    friend std::ostream& operator<<(std::ostream& out, const ConvexHull& convexHull);
    friend bool operator<(const HullPoint& one, const HullPoint& other);
    
};


#endif
