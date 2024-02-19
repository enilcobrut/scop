#pragma once

#include <iostream>

class Cam {
    public:
        explicit Cam(float distanceCamera);
        ~Cam();
        float getDistance() const;
        void setDistance(float distance);

    private:
        float distanceCamera;

};