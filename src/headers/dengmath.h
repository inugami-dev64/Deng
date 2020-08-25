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
        T x, y, z, w;

        vec4<T> operator+(const vec4<T> &vector) {
            return {(x + vector.x), (y + vector.y), (z + vector.z), (w + vector.w)};
        }

        vec4<T> operator-(const vec4<T> &vector) {
            return {(x - vector.x), (y - vector.y), (z - vector.z), (w - vector.w)};
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
            newMat.row1 = {((row1.x * matrix.row1.x) + (row1.y * matrix.row2.x) + (row1.z * matrix.row3.x) + (row1.w * matrix.row4.x)), ((row1.x * matrix.row1.y) + (row1.y * matrix.row2.y) + (row1.z * matrix.row3.y) + (row1.w * matrix.row4.y)), ((row1.x * matrix.row1.z) + (row1.y * matrix.row2.z) + (row1.z * matrix.row3.z) + (row1.w * matrix.row4.z)), ((row1.x * matrix.row1.w) + (row1.y * matrix.row2.w) + (row1.z * matrix.row3.w) + (row1.w * matrix.row4.w))};
            newMat.row2 = {((row2.x * matrix.row1.x) + (row2.y * matrix.row2.x) + (row2.z * matrix.row3.x) + (row2.w * matrix.row4.x)), ((row2.x * matrix.row1.y) + (row2.y * matrix.row2.y) + (row2.z * matrix.row3.y) + (row2.w * matrix.row4.y)), ((row2.x * matrix.row1.z) + (row2.y * matrix.row2.z) + (row2.z * matrix.row3.z) + (row2.w * matrix.row4.z)), ((row2.x * matrix.row1.w) + (row2.y * matrix.row2.w) + (row2.z * matrix.row3.w) + (row2.w * matrix.row4.w))};
            newMat.row3 = {((row3.x * matrix.row1.x) + (row3.y * matrix.row2.x) + (row3.z * matrix.row3.x) + (row3.w * matrix.row4.x)), ((row3.x * matrix.row1.y) + (row3.y * matrix.row2.y) + (row3.z * matrix.row3.y) + (row3.w * matrix.row4.y)), ((row3.x * matrix.row1.z) + (row3.y * matrix.row2.z) + (row3.z * matrix.row3.z) + (row3.w * matrix.row4.z)), ((row3.x * matrix.row1.w) + (row3.y * matrix.row2.w) + (row3.z * matrix.row3.w) + (row3.w * matrix.row4.w))};
            newMat.row4 = {((row4.x * matrix.row1.x) + (row4.y * matrix.row2.x) + (row4.z * matrix.row3.x) + (row4.w * matrix.row4.x)), ((row4.x * matrix.row1.y) + (row4.y * matrix.row2.y) + (row4.z * matrix.row3.y) + (row4.w * matrix.row4.y)), ((row4.x * matrix.row1.z) + (row4.y * matrix.row2.z) + (row4.z * matrix.row3.z) + (row4.w * matrix.row4.z)), ((row4.x * matrix.row1.w) + (row4.y * matrix.row2.w) + (row4.z * matrix.row3.w) + (row4.w * matrix.row4.w))};

            return newMat;
        }

        // theoretically this operation would be impossible, because of the lenght of vector but in our case we can just add w value of 1.0f to the end
        vec4<T> operator*(const vec3<T> vector) {
            vec4<T> newVec;
            newVec.x = {(row1.x * vector.x) + (row1.y * vector.y) + (row1.z * vector.z) + row1.w};
            newVec.y = {(row2.x * vector.x) + (row2.y * vector.y) + (row2.z * vector.z) + row2.w};
            newVec.z = {(row3.x * vector.x) + (row3.y * vector.y) + (row3.z * vector.z) + row3.w};
            newVec.w = {(row4.x * vector.x) + (row4.y * vector.y) + (row4.z * vector.z) + row4.w};

            return newVec;
        }
    };

    // generic math functions
    float degToRad(const float &deg);
    void getCirclePointCoords(const float &centre_x, const float &centre_y, const uint16_t &angle, const float &radius, float *out_x, float *out_y);
    float getFractionNumerator(const float &valNum, const float &valDenom, const float &equivalentDenom);

    class ModelMatrix {
    private:
        mat4<float> m_RxMat;
        mat4<float> m_RyMat;
        mat4<float> m_RzMat;

        mat4<float> m_transformMat;
        mat4<float> m_scaleMat;

    public:
        void setRotation(const float &x_rot, const float &y_rot, const float &z_rot);
        void setTransformation(const float &Tx, const float &Ty, const float &Tz);
        void setScale(const float &Sx, const float &Sy, const float &Sz);
        void getModelMatrix(mat4<float> *model);
    };

    class ViewMatrix {
    private:
        vec4<float> m_cameraPosition;
        mat4<float> m_transformationMat;

        float x_rot;
        float y_rot;
        float z_rot;

        vec4<float> m_rightSide;
        vec4<float> m_upSide;
        vec4<float> m_forwardSide;

        mat4<float> m_RxMat;
        mat4<float> m_RyMat;
        mat4<float> m_RzMat;

    public:
        ViewMatrix();
        void setCameraPosition(const vec4<float> &camera_pos);
        void addToPosition(const vec4<float> &movement_speed, const CoordinateType &movementType, const bool &substract);

        void setRotation(const float &x_rot, const float &y_rot);
        void setTransformationMatrix();
        void getViewMatrix(mat4<float> *viewMat);
        vec4<float> getPosition();
    };

    class ProjectionMatrix {
    private:
        float m_FOV;

        float m_aspectRatio;
        float m_near;
        float m_far;
        
        float m_top;
        float m_bottom;
        float m_left;
        float m_right;

    public:
        ProjectionMatrix(const float &FOV, const float &near,const float &far, const float &aspect_ratio);
        void getProjectionMatrix(mat4<float> *matrix);
    };

    struct UniformBufferData {
        mat4<float> model;
        mat4<float> view;
        mat4<float> projection;
    };
}

#endif