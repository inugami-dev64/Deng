#ifndef DENGMATH_H
#define DENGMATH_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <cmath>
#include <type_traits>
#include <vector>
#include <fstream>
#include <string>
#include <array>
#include <cstring>
#include <iostream>
#include <map>

namespace Deng {

    template<typename T>
    struct vec2 {
        T x, y;
    };

    template<typename T>
    struct vec3 {
        T x, y, z;
    };
    
    template<typename T>
    struct vec4 {
        T x, y, z, a;
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

    class ModelMatrix {
        private:
            double Rx;
            double Ry;
            double Rz;

            float Tx;
            float Ty;
            float Tz;

            mat4<float> modelMatrix;

            const mat4<float> RxMat = {{1.0f, 0.0f, 0.0f, 0.0f},
                                       {0.0f, cos(this->Rx), -(sin(this->Rx)), 0.0f},
                                       {0.0f, sin(this->Rx), cos(this->Rx), 0.0f}, 
                                       {0.0f, 0.0f, 0.0f, 1.0f}};

            const mat4<float> RyMat = {{cos(this->Ry), 0.0f, sin(this->Ry), 0.0f},
                                       {0.0f, 1.0f, 0.0f, 0.0f},
                                       {-(sin(this->Ry)), 0.0f, cos(this->Ry), 0.0f},
                                       {0.0f, 0.0f, 0.0f, 1.0f}};

            const mat4<float> RzMat = {{cos(this->Rz), -(sin(this->Rz)), 0.0f, 0.0f},
                                       {sin(this->Rz), cos(this->Rz), 0.0f, 0.0f},
                                       {0.0f, 0.0f, 1.0f, 0.0f}, 
                                       {0.0f, 0.0f, 0.0f, 1.0f}};

            const mat4<float> transformMat = {{1.0f, 0.0f, 0.0f, this->Tx}, 
                                              {0.0f, 1.0f, 0.0f, this->Ty},
                                              {0.0f, 0.0f, 1.0f, this->Tz},
                                              {0.0f, 0.0f, 0.0f, 1.0f}};

        public:
            void setRotation(const double &Rx, const double &Ry, const double &Rz);
            void setTransformation();

    };
}

#endif