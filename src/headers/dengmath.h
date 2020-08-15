#ifndef DENGMATH_H
#define DENGMATH_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vector>
#include <fstream>
#include <string>
#include <array>
#include <cstring>
#include <iostream>
#include <glm/glm.hpp>
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
    struct mat3 {
        vec3<T> line1;
        vec3<T> line2;
        vec3<T> line3;
    };

    template<typename T>
    struct mat2x3 {
        vec2<T> line1;
        vec2<T> line2;
        vec2<T> line3; 
    };
}

#endif