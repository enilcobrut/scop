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
     Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

     Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3& operator/=(float value) {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }
static Vec3 calculateObjectCenter(const std::vector<float>& vertices) {
        Vec3 center(0.0f, 0.0f, 0.0f);
        int count = 0;

        for (size_t i = 0; i < vertices.size(); i += 6) { // Supposons 6 valeurs par sommet (x, y, z, nx, ny, nz)
            center += Vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
            count++;
        }

        if (count > 0) {
            center /= static_cast<float>(count);
        }

        return center;
    }

 static Vec3 calculateDimensions(const std::vector<float>& vertices) {
    if (vertices.empty()) return Vec3(0.0f, 0.0f, 0.0f);

    float minX = vertices[0], maxX = vertices[0];
    float minY = vertices[1], maxY = vertices[1];
    float minZ = vertices[2], maxZ = vertices[2];

    for (size_t i = 0; i < vertices.size(); i += 6) {
        minX = std::min(minX, vertices[i]);
        maxX = std::max(maxX, vertices[i]);
        minY = std::min(minY, vertices[i + 1]);
        maxY = std::max(maxY, vertices[i + 1]);
        minZ = std::min(minZ, vertices[i + 2]);
        maxZ = std::max(maxZ, vertices[i + 2]);
    }

    return Vec3(maxX - minX, maxY - minY, maxZ - minZ);
}

// Dans votre classe Vec3, ajoutez cette méthode :
Vec3 operator*(float scalar) const {
    return Vec3(x * scalar, y * scalar, z * scalar);
}




      Vec3 normalize() const {
        float length = std::sqrt(x * x + y * y + z * z);
        return Vec3(x / length, y / length, z / length);
    }

    // Produit vectoriel
    static Vec3 cross(const Vec3& a, const Vec3& b) {
        return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }

    // Produit scalaire
    static float dot(const Vec3& a, const Vec3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

};

