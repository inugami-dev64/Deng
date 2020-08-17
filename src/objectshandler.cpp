#include "headers/renderer.h"

namespace deng {
    VkVertexInputBindingDescription VertexInputDesc::getBindingDesc(GameObject &obj) {
        VkVertexInputBindingDescription local_input_binding_desc{};
        local_input_binding_desc.binding = 0;
        local_input_binding_desc.stride = sizeof(obj.vertexData[0]);
        local_input_binding_desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return local_input_binding_desc;
    } 

    std::array<VkVertexInputAttributeDescription, 2> VertexInputDesc::getAttributeDesc() {
        std::array<VkVertexInputAttributeDescription, 2> local_input_attr_desc{};

        local_input_attr_desc[0].binding = 0;
        local_input_attr_desc[0].location = 0;
        local_input_attr_desc[0].format = VK_FORMAT_R32G32_SFLOAT;
        local_input_attr_desc[0].offset = offsetof(ObjVertexData, posVec);

        local_input_attr_desc[1].binding = 0;
        local_input_attr_desc[1].location = 1;
        local_input_attr_desc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        local_input_attr_desc[1].offset = offsetof(ObjVertexData, colorVec);

        return local_input_attr_desc;
    }

}