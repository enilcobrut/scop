#pragma once

#include <vector>
#include <cstdlib>
class Vec3 {
public:
    float x, y, z;

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float xValue, float yValue, float zValue) : x(xValue), y(yValue), z(zValue) {}

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }


    Vec3 operator-() const {
        return Vec3(-x, -y, -z);
    }


    Vec3& operator/=(float value) {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }
    static Vec3 calculateObjectCenter(const std::vector<float>& vertices) {
        Vec3 center;
        int count = 0;

        for (size_t i = 0; i < vertices.size(); i += 6) {
            center += Vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
            count++;
        }

        if (count > 0) {
            center /= count;
        }
        return center;
    }
    static Vec3 calculateDimensions(const std::vector<float>& vertices) {
    if (vertices.empty()) return Vec3();

    float minX = vertices[0], maxX = vertices[0];
        float minY = vertices[1], maxY = vertices[1];
        float minZ = vertices[2], maxZ = vertices[2];

        for (size_t i = 0; i < vertices.size(); i += 6) { // Supposons que chaque vertex a 6 valeurs (x, y, z, nx, ny, nz)
            minX = std::min(minX, vertices[i]);
            maxX = std::max(maxX, vertices[i]);
            minY = std::min(minY, vertices[i + 1]);
            maxY = std::max(maxY, vertices[i + 1]);
            minZ = std::min(minZ, vertices[i + 2]);
            maxZ = std::max(maxZ, vertices[i + 2]);
        }

        return Vec3(maxX - minX, maxY - minY, maxZ - minZ);
    }

};
