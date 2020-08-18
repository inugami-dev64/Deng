#ifndef OBJECTSHANDLER_H
#define OBJECTSHANDLER_H
#include "textureloader.h"

namespace deng {

    struct ObjVertexData {
        vec3<float> posVec;
        vec4<float> colorVec;     
    };

    struct ObjVertexIndicesData {
        std::vector<uint32_t> posIndices;
        std::vector<uint32_t> texIndices;
    };

    struct ObjTextureData {
        std::vector<vec4<uint8_t>> texturePixelsData;

        uint32_t width;
        uint32_t height;
        VkDeviceSize texSize;
    };

    struct Buffers {
        VkBuffer vertex_buffer;
        VkDeviceMemory vertex_bufferMem;

        VkBuffer staging_buffer;
        VkDeviceMemory staging_bufferMem;

        VkBuffer index_buffer;
        VkDeviceMemory index_bufferMem;
    };

    struct Images {
        VkImage textureImage;
        VkDeviceMemory textureImageMem;
    };

    struct GameObject {
        vec3<float> origin;
        std::vector<ObjVertexData> vertexData;
        ObjVertexIndicesData vertexIndicesData;
        ObjTextureData textureData;
        ModelMatrix modelMatrix;
        
        Images images;
        Buffers buffers;
    };

    struct VertexInputDesc {
        static VkVertexInputBindingDescription getBindingDesc(GameObject &obj);
        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDesc();
    };
}

#endif