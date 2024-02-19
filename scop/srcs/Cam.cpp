#include "Cam.hpp"

Cam::Cam(float distance) : distanceCamera(distance) {}

Cam::~Cam() {}

float Cam::getDistance() const {
    return distanceCamera;
}

void Cam::setDistance(float distance) {
    distanceCamera = distance;
}