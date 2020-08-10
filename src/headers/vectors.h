#ifndef VECTORS_H
#define VECTORS_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

namespace Deng {
    class Vector2D {
        public:
            uint32_t x;
            uint32_t y;
    };

    class Vector3D {
        public:
            uint32_t x;
            uint32_t y;
            uint32_t z;
    };
}

#endif