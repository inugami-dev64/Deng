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

    enum ShaderType {
        DENG_SHADER_VERTEX  = 0,
        DENG_SHADER_FRAG = 1
    };

    struct vec2 {
        float x;
        float y;
    };

    struct vec3 {
        float x;
        float y;
        float z;
    };

    struct vec4 {
        float x;
        float y;
        float z;
        float a;
    };

    struct Vertex {
        vec3 pos;
        vec3 colors;

        static VkVertexInputBindingDescription getBindingDesc();
        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDesc();
    };
}

#endif