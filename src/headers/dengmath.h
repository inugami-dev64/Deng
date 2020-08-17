#ifndef DENGMATH_H
#define DENGMATH_H
#define PI 3.1415926

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <cmath>
#include <chrono>
#include <type_traits>
#include <vector>
#include <fstream>
#include <string>
#include <array>
#include <cstring>
#include <iostream>
#include <map>

namespace deng {

    enum DengBool {
        DENG_FALSE = 0,
        DENG_TRUE = 1
    };

    enum CoordinateType {
        DENG_X = 0,
        DENG_Y = 1,
        DENG_Z = 2
    };

    template<typename T>
    struct vec2 {
        T x, y;

        vec2<T> operator+(const vec2<T> &vector) {
            return {(x + vector.x), (y + vector.y)};
        }

        vec2<T> operator-(const vec2<T> &vector) {
            return {(x - vector.x), (y - vector.y)};
        }
    };

    template<typename T>
    struct vec3 {
        T x, y, z;
        
        vec3<T> operator+(const vec3<T> &vector) {
            return {(x + vector.x), (y + vector.y), (z + vector.z)};
        }

        vec3<T> operator-(const vec3<T> &vector) {
            return {(x - vector.x), (y - vector.y), (z - vector.z)};
        }
    };
    
    template<typename T>
    struct vec4 {
        T x, y, z, a;

        vec4<T> operator+(const vec4<T> &vector) {
            return {(x + vector.x), (y + vector.y), (z + vector.z), (a + vector.a)};
        }

        vec4<T> operator-(const vec4<T> &vector) {
            return {(x - vector.x), (y - vector.y), (z - vector.z), (a - vector.a)};
        }
    };

    template<typename T>
    struct mat2 {
        vec2<T> row1;
        vec2<T> row2;
    };
    
    template<typename T>
    struct mat3x2 {
        vec2<T> row1;
        vec2<T> row2;
        vec2<T> row3;
    };

    template<typename T>
    struct mat3 {
        vec3<T> row1;
        vec3<T> row2;
        vec3<T> row3;

        mat3<T> operator*(const mat3<T> &matrix) {
            mat3<T> newMat;
            newMat.row1 = {((row1.x * matrix.row1.x) + (row1.y * matrix.row2.x) + (row1.z * matrix.row3.x)), ((row1.x * matrix.row1.y) + (row1.y * matrix.row2.y) + (row1.z * matrix.row3.y)), ((row1.x * matrix.row1.z) + (row1.y * matrix.row2.z) + (row1.z * matrix.row3.z))};
            newMat.row2 = {((row2.x * matrix.row1.x) + (row2.y * matrix.row2.x) + (row2.z * matrix.row3.x)), ((row2.x * matrix.row1.y) + (row2.y * matrix.row2.y) + (row2.z * matrix.row3.y)), ((row2.x * matrix.row1.z) + (row2.y * matrix.row2.z) + (row2.z * matrix.row3.z))};
            newMat.row3 = {((row3.x * matrix.row1.x) + (row3.y * matrix.row2.x) + (row3.z * matrix.row3.x)), ((row3.x * matrix.row1.y) + (row3.y * matrix.row2.y) + (row3.z * matrix.row3.y)), ((row3.x * matrix.row1.z) + (row3.y * matrix.row2.z) + (row3.z * matrix.row3.z))};

            return newMat;
        }
    };

    template<typename T>
    struct mat4 {
        vec4<T> row1;
        vec4<T> row2;
        vec4<T> row3;
        vec4<T> row4; 

        mat4<T> operator*(const mat4<T> &matrix) {
            mat4<T> newMat;
            newMat.row1 = {((row1.x * matrix.row1.x) + (row1.y * matrix.row2.x) + (row1.z * matrix.row3.x) + (row1.a * matrix.row4.x)), ((row1.x * matrix.row1.y) + (row1.y * matrix.row2.y) + (row1.z * matrix.row3.y) + (row1.a * matrix.row4.y)), ((row1.x * matrix.row1.z) + (row1.y * matrix.row2.z) + (row1.z * matrix.row3.z) + (row1.a * matrix.row4.z)), ((row1.x * matrix.row1.a) + (row1.y * matrix.row2.a) + (row1.z * matrix.row3.a) + (row1.a * matrix.row4.a))};
            newMat.row2 = {((row2.x * matrix.row1.x) + (row2.y * matrix.row2.x) + (row2.z * matrix.row3.x) + (row2.a * matrix.row4.x)), ((row2.x * matrix.row1.y) + (row2.y * matrix.row2.y) + (row2.z * matrix.row3.y) + (row2.a * matrix.row4.y)), ((row2.x * matrix.row1.z) + (row2.y * matrix.row2.z) + (row2.z * matrix.row3.z) + (row2.a * matrix.row4.z)), ((row2.x * matrix.row1.a) + (row2.y * matrix.row2.a) + (row2.z * matrix.row3.a) + (row2.a * matrix.row4.a))};
            newMat.row3 = {((row3.x * matrix.row1.x) + (row3.y * matrix.row2.x) + (row3.z * matrix.row3.x) + (row3.a * matrix.row4.x)), ((row3.x * matrix.row1.y) + (row3.y * matrix.row2.y) + (row3.z * matrix.row3.y) + (row3.a * matrix.row4.y)), ((row3.x * matrix.row1.z) + (row3.y * matrix.row2.z) + (row3.z * matrix.row3.z) + (row3.a * matrix.row4.z)), ((row3.x * matrix.row1.a) + (row3.y * matrix.row2.a) + (row3.z * matrix.row3.a) + (row3.a * matrix.row4.a))};
            newMat.row4 = {((row4.x * matrix.row1.x) + (row4.y * matrix.row2.x) + (row4.z * matrix.row3.x) + (row4.a * matrix.row4.x)), ((row4.x * matrix.row1.y) + (row4.y * matrix.row2.y) + (row4.z * matrix.row3.y) + (row4.a * matrix.row4.y)), ((row4.x * matrix.row1.z) + (row4.y * matrix.row2.z) + (row4.z * matrix.row3.z) + (row4.a * matrix.row4.z)), ((row4.x * matrix.row1.a) + (row4.y * matrix.row2.a) + (row4.z * matrix.row3.a) + (row4.a * matrix.row4.a))};

            return newMat;
        }
    };

    // generic conversion functions
    float degToRad(const uint16_t &deg) {
        return (deg/360) * 2 * PI;
    }

    struct CameraMath {
        void getMaxCoords(const float &currentRotationXY, const float &FOV, const float &draw_distance, vec2<float> *camera_max_centre_coords, vec2<float> *camera_max_left_corner_coord, vec2<float> *camera_max_right_corner_coord);

    };

    class ModelMatrix {
    private:
        mat4<float> RxMat;
        mat4<float> RyMat;
        mat4<float> RzMat;

        mat4<float> transformMat;
        mat4<float> scaleMat;

    public:
        void setRotation(const uint16_t &Rx, const uint16_t &Ry, const uint16_t &Rz);
        void setTransformation(const float &Tx, const float &Ty, const float &Tz);
        void setScale(const float &Sx, const float &Sy, const float &Sz);
        void getMatrix(mat4<float> *model);
    };

    class ViewMatrix {
    private:
        vec4<float> rightSide;
        vec4<float> forwardSide;
        vec4<float> upSide;
        vec4<float> cameraPosition;

    public:
        void setAxes(const vec4<float> &right, const vec4<float> &forward, const vec4<float> &up);
        void setPosition(const vec4<float> &cam_Position);
        void addToPosition(const vec4<float> &addition, const CoordinateType &type, const bool &substract);
        void getViewMatrix(mat4<float> *view);
    };

    class ProjectionMatrix {
    private:

    };
}

#endif