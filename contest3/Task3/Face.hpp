#ifndef Face_hpp
#define Face_hpp

#include <iostream>

class Face {
private:
    size_t first;
    size_t second;
    size_t third;
public:
    static const int NUMBER_OF_VERTEXES = 6;
    
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

#endif
