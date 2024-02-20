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

    static Mat4 perspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
        Mat4 result;

        // Calcul des éléments de la matrice de perspective
        float tanHalfFovy = tan(fov / 2.0f);
        result[0][0] = 1.0f / (aspectRatio * tanHalfFovy);
        result[1][1] = 1.0f / tanHalfFovy;
        result[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        result[2][3] = -1.0f;
        result[3][2] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
        result[3][3] = 0.0f;

        return result;
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
