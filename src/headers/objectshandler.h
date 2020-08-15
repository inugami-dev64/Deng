#ifndef OBJECTSHANDLER_H
#define OBJECTSHANDLER_H
#include "textureloader.h"

namespace Deng {

    enum AttributeDescOffsetType {
        DENG_ATTRIBUTE_OFFSET_TYPE_VERTICES_COORD = 0,
        DENG_ATTRIBUTE_OFFSET_TYPE_VERTICES_TEXTURE_COORD = 1
    };

    struct ObjVertexData {
        vec3<float> posVec;
        vec4<float> colorVec; 
        vec2<float> textureVec;    
    };

    struct ObjTextureData {
        std::vector<vec4<unsigned char>> texturePixelsData;

        uint32_t width;
        uint32_t height;
        VkDeviceSize texSize;
    };

    struct Buffers {
        VkBuffer vertex_buffer;
        VkDeviceMemory vertex_bufferMem;

        VkBuffer staging_buffer;
        VkDeviceMemory staging_bufferMem;
    };

    struct Images {
        VkImage textureImage;
        VkDeviceMemory textureImageMem;
    };

    struct GameObject {
        std::vector<ObjVertexData> vertexData;
        ObjTextureData textureData;
        
        Images images;
        Buffers buffers;
    };

    struct VertexInputDesc {
        static VkVertexInputBindingDescription getBindingDesc(GameObject &obj);
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDesc();
    };
}

#endif