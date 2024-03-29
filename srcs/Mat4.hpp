#pragma once
#include <array>
#include "Vec3.hpp"
#include <cmath>
class Mat4 {
    public:
        std::array<std::array<float, 4>, 4> data;

        Mat4() {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    data[i][j] = (i == j) ? 1.0f : 0.0f;
        }

        float* operator[](int index) {
            return data[index].data();
        }

        const float* operator[](int index) const {
            return data[index].data();
        }

        static Mat4 translate(const Vec3& translation) {
            Mat4 result;
            result[3][0] = translation.x;
            result[3][1] = translation.y;
            result[3][2] = translation.z;
            return result;
        }

        static Mat4 rotate(float angle, const Vec3& axis) {
        Mat4 result;
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);
        float oneMinusCos = 1.0f - cosAngle;

        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

        result[0][0] = x * x * oneMinusCos + cosAngle;
        result[0][1] = x * y * oneMinusCos - z * sinAngle;
        result[0][2] = x * z * oneMinusCos + y * sinAngle;
        
        result[1][0] = y * x * oneMinusCos + z * sinAngle;
        result[1][1] = y * y * oneMinusCos + cosAngle;
        result[1][2] = y * z * oneMinusCos - x * sinAngle;
        
        result[2][0] = x * z * oneMinusCos - y * sinAngle;
        result[2][1] = y * z * oneMinusCos + x * sinAngle;
        result[2][2] = z * z * oneMinusCos + cosAngle;

        return result;
    }

    static Mat4 lookAt(Vec3 position, Vec3 target, Vec3 worldUp) {
    Vec3 zAxis = (position - target).normalize();
    Vec3 xAxis = Vec3::cross(worldUp, zAxis).normalize();
    Vec3 yAxis = Vec3::cross(zAxis, xAxis);


    Mat4 viewMatrix;
    viewMatrix[0][0] = xAxis.x;
    viewMatrix[1][0] = xAxis.y;
    viewMatrix[2][0] = xAxis.z;
    viewMatrix[0][1] = yAxis.x;
    viewMatrix[1][1] = yAxis.y;
    viewMatrix[2][1] = yAxis.z;
    viewMatrix[0][2] = zAxis.x;
    viewMatrix[1][2] = zAxis.y;
    viewMatrix[2][2] = zAxis.z;
    viewMatrix[3][0] = -Vec3::dot(xAxis, position);
    viewMatrix[3][1] = -Vec3::dot(yAxis, position);
    viewMatrix[3][2] = -Vec3::dot(zAxis, position);


    return viewMatrix;
}
 static Mat4 zero() {
        Mat4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.data[i][j] = 0.0f;
            }
        }
        return result;
    }

   static Mat4 perspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
    Mat4 result = Mat4::zero(); // Assurez-vous que cette méthode initialise tous les éléments à 0

    float tanHalfFovy = tan(fov / 2.0f);
    result[0][0] = 1.0f / (aspectRatio * tanHalfFovy);
    result[1][1] = 1.0f / tanHalfFovy;
    result[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    result[2][3] = -1.0f;
    result[3][2] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
    // result[3][3] est déjà à 0

    return result;
}

    static float radians(float degrees) {
        return degrees * static_cast<float>(M_PI) / 180.0f;
    }
    const float* value_ptr() const {
        return &data[0][0];
    }

    Mat4 operator*(const Mat4& other) const {
            Mat4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result[i][j] = 0.0f;
                    for (int k = 0; k < 4; ++k) {
                        result[i][j] += data[i][k] * other.data[k][j];
                    }
                }
            }
            return result;
        }

        static Mat4 scale(const Vec3& scaleFactor) {
        Mat4 result;
        result[0][0] = scaleFactor.x;
        result[1][1] = scaleFactor.y;
        result[2][2] = scaleFactor.z;
        return result;
    }



};
