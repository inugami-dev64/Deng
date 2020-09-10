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
#include <typeinfo>

/*  BEFORE EVERY GIT COMMIT SET DEBUG VALUE TO 0!!!     */

#define DEBUG 1
#define ERR(x) throw std::runtime_error(x)
#define ERRME(x) std::cout << "\033[1;31m" << x << "\033[0m\n" 

#if DEBUG
    #define LOG(x) std::cout << x << "\n"
    const bool enable_validation_layers = true;
#else
    #define LOG(x)
    const bool enable_validation_layers = false;
#endif

// this is quite a mess
namespace deng {

    enum dengBool {
        DENG_FALSE = false,
        DENG_TRUE = true
    };

    enum dengCoordinateType {
        DENG_X = 0,
        DENG_Y = 1,
        DENG_Z = 2
    };

    template<typename T>
    struct vec2 {
        T first, second;
        size_t size() {
            return static_cast<size_t>(2);
        }

        T *getVectorElement(const int &i) {
            T *local_current_element;
            switch (i)
            {
            case 0:
                local_current_element = &this->first;
                break;
            
            case 1:
                local_current_element = &this->second;
                break;
            
            default:
                break;
            }
            return local_current_element;
        }

        vec2<T> operator+(const vec2<T> &vector) {
            vec2<T> local_vec2;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                local_vec2 = {(first + vector.first), (second + vector.second)};
            }

            return local_vec2;
        }

        vec2<T> operator-(const vec2<T> &vector) {
            vec2<T> local_vec2;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                local_vec2 = {(first - vector.first), (second - vector.second)};
            }
            
            return local_vec2;
        }
    };

    template<typename T>
    struct vec3 {
        T first, second, third;
        size_t size() {
            return static_cast<size_t>(3);
        }

        T *getVectorElement(const int &i) {
            T *local_current_element;
            switch (i)
            {
            case 0:
                local_current_element = &this->first;
                break;
            
            case 1:
                local_current_element = &this->second;
                break;

            case 2:
                local_current_element = &this->third;
                break;
            
            default:
                break;
            }
            return local_current_element;
        }

        vec3<T> operator+(const vec3<T> &vector) {
            vec3<T> local_vec3;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                local_vec3 = {(first + vector.first), (second + vector.second), (third + vector.third)};
            }

            return local_vec3;
        }

        vec3<T> operator-(const vec3<T> &vector) {
            vec3<T> local_vec3;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                local_vec3 = {(first - vector.first), (second - vector.second), (third - vector.third)};
            }

            return local_vec3;
        }
    };
    
    template<typename T>
    struct vec4 {
        T first, second, third, fourth;
        size_t size() {
            return static_cast<size_t>(4);
        }

        T *getVectorElement(const int &i) {
            T *local_current_element;
            switch (i)
            {
            case 0:
                local_current_element = &this->first;
                break;
            
            case 1:
                local_current_element = &this->first;
                break;

            case 2:
                local_current_element = &this->third;
                break;

            case 3:
                local_current_element = &this->fourth;
                break;
            
            default:
                break;
            }
            return &local_current_element;
        }

        vec4<T> operator+(const vec4<T> &vector) {
            vec4<T> local_vec4;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                local_vec4 = {(first + vector.first), (second + vector.second), (third + vector.third), (fourth + vector.fourth)};
            }

            return local_vec4;
        }

        vec4<T> operator-(const vec4<T> &vector) {
            vec4<T> local_vec4;
            if(typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(uint8_t).name() || typeid(T).name() == typeid(uint16_t).name() || typeid(T).name() == typeid(uint32_t).name() || typeid(T).name() == typeid(uint64_t).name() || typeid(T).name() == typeid(int).name() || typeid(T).name() == typeid(double).name()) {
                local_vec4 = {(first - vector.first), (second - vector.second), (third - vector.third), (fourth - vector.fourth)};
            }

            return local_vec4;
        }
    };
    
    template<typename T>
    
    class DengIterator {

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
            newMat.row1 = {((row1.first * matrix.row1.first) + (row1.second * matrix.row2.first) + (row1.third * matrix.row3.first)), ((row1.first * matrix.row1.second) + (row1.second * matrix.row2.second) + (row1.third * matrix.row3.second)), ((row1.first * matrix.row1.third) + (row1.second * matrix.row2.third) + (row1.third * matrix.row3.third))};
            newMat.row2 = {((row2.first * matrix.row1.first) + (row2.second * matrix.row2.first) + (row2.third * matrix.row3.first)), ((row2.first * matrix.row1.second) + (row2.second * matrix.row2.second) + (row2.third * matrix.row3.second)), ((row2.first * matrix.row1.third) + (row2.second * matrix.row2.third) + (row2.third * matrix.row3.third))};
            newMat.row3 = {((row3.first * matrix.row1.first) + (row3.second * matrix.row2.first) + (row3.third * matrix.row3.first)), ((row3.first * matrix.row1.second) + (row3.second * matrix.row2.second) + (row3.third * matrix.row3.second)), ((row3.first * matrix.row1.third) + (row3.second * matrix.row2.third) + (row3.third * matrix.row3.third))};

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
            newMat.row1 = {((row1.first * matrix.row1.first) + (row1.second * matrix.row2.first) + (row1.third * matrix.row3.first) + (row1.fourth * matrix.row4.first)), ((row1.first * matrix.row1.second) + (row1.second * matrix.row2.second) + (row1.third * matrix.row3.second) + (row1.fourth * matrix.row4.second)), ((row1.first * matrix.row1.third) + (row1.second * matrix.row2.third) + (row1.third * matrix.row3.third) + (row1.fourth * matrix.row4.third)), ((row1.first * matrix.row1.fourth) + (row1.second * matrix.row2.fourth) + (row1.third * matrix.row3.fourth) + (row1.fourth * matrix.row4.fourth))};
            newMat.row2 = {((row2.first * matrix.row1.first) + (row2.second * matrix.row2.first) + (row2.third * matrix.row3.first) + (row2.fourth * matrix.row4.first)), ((row2.first * matrix.row1.second) + (row2.second * matrix.row2.second) + (row2.third * matrix.row3.second) + (row2.fourth * matrix.row4.second)), ((row2.first * matrix.row1.third) + (row2.second * matrix.row2.third) + (row2.third * matrix.row3.third) + (row2.fourth * matrix.row4.third)), ((row2.first * matrix.row1.fourth) + (row2.second * matrix.row2.fourth) + (row2.third * matrix.row3.fourth) + (row2.fourth * matrix.row4.fourth))};
            newMat.row3 = {((row3.first * matrix.row1.first) + (row3.second * matrix.row2.first) + (row3.third * matrix.row3.first) + (row3.fourth * matrix.row4.first)), ((row3.first * matrix.row1.second) + (row3.second * matrix.row2.second) + (row3.third * matrix.row3.second) + (row3.fourth * matrix.row4.second)), ((row3.first * matrix.row1.third) + (row3.second * matrix.row2.third) + (row3.third * matrix.row3.third) + (row3.fourth * matrix.row4.third)), ((row3.first * matrix.row1.fourth) + (row3.second * matrix.row2.fourth) + (row3.third * matrix.row3.fourth) + (row3.fourth * matrix.row4.fourth))};
            newMat.row4 = {((row4.first * matrix.row1.first) + (row4.second * matrix.row2.first) + (row4.third * matrix.row3.first) + (row4.fourth * matrix.row4.first)), ((row4.first * matrix.row1.second) + (row4.second * matrix.row2.second) + (row4.third * matrix.row3.second) + (row4.fourth * matrix.row4.second)), ((row4.first * matrix.row1.third) + (row4.second * matrix.row2.third) + (row4.third * matrix.row3.third) + (row4.fourth * matrix.row4.third)), ((row4.first * matrix.row1.fourth) + (row4.second * matrix.row2.fourth) + (row4.third * matrix.row3.fourth) + (row4.fourth * matrix.row4.fourth))};

            return newMat;
        }

            // theoretically this operation would be impossible, because of the lenght of vector but in our case we can just add w value of 1.0f to the end
        vec4<T> operator*(const vec3<T> vector) {
            vec4<T> newVec;
            newVec.first = {(row1.first * vector.first) + (row1.second * vector.second) + (row1.third * vector.third) + row1.fourth};
            newVec.second = {(row2.first * vector.first) + (row2.second * vector.second) + (row2.third * vector.third) + row2.fourth};
            newVec.third = {(row3.first * vector.first) + (row3.second * vector.second) + (row3.third * vector.third) + row3.fourth};
            newVec.fourth = {(row4.first * vector.first) + (row4.second * vector.second) + (row4.third * vector.third) + row4.fourth};

            return newVec;
        }
    };

    // generic math functions
    float degToRad(const float &deg);
    void getCirclePointCoords(const float &centre_x, const float &centre_y, const uint16_t &angle, const float &radius, float *out_x, float *out_y);
    float getFractionNumerator(const float &value_numerator, const float &value_denominator, const float &equivalent_denominator);

    class ModelMatrix {
    private:
        mat4<float> m_rot_x_mat;
        mat4<float> m_rot_y_mat;
        mat4<float> m_rot_z_mat;

        mat4<float> m_transformation_mat;
        mat4<float> m_scale_mat;

    public:
        void setRotation(const float &x_rot, const float &y_rot, const float &z_rot);
        void setTransformation(const float &transform_x, const float &transform_y, const float &transform_z);
        void setScale(const float &scale_x, const float &scale_y, const float &scale_z);
        void getModelMatrix(mat4<float> *p_model);
    };

    class ViewMatrix {
    private:
        vec4<float> m_camera_position;
        mat4<float> m_transformation_mat;

        float x_rot;
        float y_rot;
        float z_rot;

        vec4<float> m_right_side;
        vec4<float> m_up_side;
        vec4<float> m_forward_side;

        mat4<float> m_rot_x_mat;
        mat4<float> m_rot_y_mat;
        mat4<float> m_rot_z_mat;

    public:
        ViewMatrix();
        void setCameraPosition(const vec4<float> &camera_pos);
        void addToPosition(const vec4<float> &movement_speed, const dengCoordinateType &movement_type, const bool &substract);

        void setRotation(const float &x_rot, const float &y_rot);
        void setTransformationMatrix();
        void getViewMatrix(mat4<float> *view_mat);
        vec4<float> getPosition();
    };

    class ProjectionMatrix {
    private:
        float m_FOV;

        float m_aspect_ratio;
        float m_near;
        float m_far;

    public:
        ProjectionMatrix(const float &FOV, const float &near, const float &far, const float &aspect_ratio);
        void getProjectionMatrix(mat4<float> *p_matrix);
    };

    struct UniformBufferData {
        mat4<float> model;
        mat4<float> view;
        mat4<float> projection;
    };
}

#endif