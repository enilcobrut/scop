#include "Point.hpp"
#include <cmath> 
Point::Point(const int indexV, int indexVt, int indexVn, std::array<float, 3>coordVertex) : indexV(indexV), indexVt(indexVt), indexVn(indexVn), coordVertex(coordVertex) {
    if (indexVn == -1) {
        this->normPoint = getNorm();
    }
}


std::array<float, 3> Point::getNorm() const {
    float norm = std::sqrt(coordVertex[0] * coordVertex[0] +
                           coordVertex[1] * coordVertex[1] +
                           coordVertex[2] * coordVertex[2]);

    return {norm, norm, norm}; 
}

void Point::setIndexVn(int vn) {


    indexVn = vn;
}

void Point::setIndexVt(int vn) {
    indexVt = vn;
}

Point::~Point() {

}