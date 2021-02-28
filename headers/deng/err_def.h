#ifndef ERR_DEF_H
#define ERR_DEF_H

// Console loggers
#if __GENERIC_DEBUG
    #define LOG(x) std::cout << "LOG: " << x << std::endl
    #define WARNME(x) std::cout << "WARNING: " << x << std::endl
#else
    #define LOG(x)
    #define WARNME(x)
    #define ERRME(x)
#endif


// Runtime errors
#define MEM_ERR(x)              throw std::runtime_error(std::string("Failed to allocate memory: ") + x)
#define MATH_ERR(x)             throw std::runtime_error(std::string("Math error: ") + x)
#define FONT_ERR(x)             throw std::runtime_error(std::string("Font rasterisation error: ") + x)
#define RUN_ERR(x)              throw std::runtime_error(x)

// Vulkan related
#define VK_GEN_ERR(x)           throw std::runtime_error(std::string("Vulkan general error: ") + x)
#define VK_INSTANCE_ERR(x)      throw std::runtime_error(std::string("Vulkan instance error: ") + x)
#define VK_SWAPCHAIN_ERR(x)     throw std::runtime_error(std::string("Vulkan swapchain error: ") + x)
#define VK_DESC_ERR(x)          throw std::runtime_error(std::string("Vulkan descriptor error: ") + x)
#define VK_RES_ERR(x)           throw std::runtime_error(std::string("Vulkan resource allocation error: ") + x)
#define VK_DRAWCMD_ERR(x)       throw std::runtime_error(std::string("Vulkan draw command error: ") + x)
#define VK_PIPELINEC_ERR(x)     throw std::runtime_error(std::string("Vulkan pipeline creation error: ") + x)
#define VK_BUFFER_ERR(x)        throw std::runtime_error(std::string("Vulkan buffer error: ") + x)
#define VK_FRAME_ERR(x)         throw std::runtime_error(std::string("Vulkan frame error: ") + x)
#define VK_VAL_LAYER(x)         std::cout << "VL: " << x << std::endl

#endif
