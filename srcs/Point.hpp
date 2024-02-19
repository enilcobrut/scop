#pragma once
#include <iostream>
#include <vector>
#include <array>
class Point {
    public:
        Point(const int indexV,  int indexVt,  int indexVn, std::array<float, 3> coordVertex);
        void setIndexVn(int vn);
        void setIndexVt(int vt);
        int getIndexV() const { return indexV; }
        int getIndexVt() const { return indexVt; }
        int getIndexVn() const { return indexVn; }
        std::array<float, 3> getNorm() const;
        ~Point();

    private:
        Point();
        const int indexV;
        int indexVt;
        int indexVn;
        std::array<float, 3> coordVertex;
        std::array<float, 3> normPoint;
};