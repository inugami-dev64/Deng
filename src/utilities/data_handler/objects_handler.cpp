#include "../../core/deng_core.h"

namespace dengUtils {

    ObjRawTextureData::ObjRawTextureData() {
        this->p_width = new uint32_t;
        this->p_height = new uint32_t;
        this->p_texture_size = new VkDeviceSize;
    }

    void ObjRawTextureData::cpyDims(ObjTextureData &texData) {
        texData.width = *this->p_width; 
        texData.height = *this->p_height; 
    }

    void ObjRawTextureData::clear() {
        this->texture_pixels_data.clear();
        delete this->p_height;
        delete this->p_width;
        delete this->p_texture_size;
    }
}