#include "../core/dengcore.h"

namespace dengUI {
    

    void VerticesDataCreator::getRectangleVertices(RectangleSpriteInfo *p_sprite_info, std::vector<UIVerticesData> &vertices_data, std::vector<uint16_t> &indices_data) {
        if(p_sprite_info != nullptr) {
            vertices_data.resize(4);
            
            vertices_data[0].position_vec.first = p_sprite_info->position.first - (((p_sprite_info->origin.first * 0.5) + 0.5) * p_sprite_info->size.first);
            vertices_data[0].position_vec.second = p_sprite_info->position.second - (((p_sprite_info->origin.second * 0.5) + 0.5) * p_sprite_info->size.second);
            vertices_data[0].position_vec.third = 0.0f;

            vertices_data[1].position_vec.first = p_sprite_info->position.first + (1 - (((p_sprite_info->origin.first * 0.5) + 0.5) * p_sprite_info->size.first));
            vertices_data[1].position_vec.second = p_sprite_info->position.second - (((p_sprite_info->origin.second * 0.5) + 0.5) * p_sprite_info->size.second);
            vertices_data[1].position_vec.third = 0.0f;

            vertices_data[2].position_vec.first = p_sprite_info->position.first + (1 - (((p_sprite_info->origin.first * 0.5) + 0.5) * p_sprite_info->size.first));
            vertices_data[2].position_vec.second = p_sprite_info->position.second + (1 - (((p_sprite_info->origin.second * 0.5) + 0.5) * p_sprite_info->size.second));
            vertices_data[2].position_vec.third = 0.0f;

            vertices_data[3].position_vec.first = p_sprite_info->position.first - (((p_sprite_info->origin.first * 0.5) + 0.5) * p_sprite_info->size.first);
            vertices_data[3].position_vec.second = p_sprite_info->position.second + (1 - (((p_sprite_info->origin.second * 0.5) + 0.5) * p_sprite_info->size.second));
            vertices_data[3].position_vec.third = 0.0f;

            for(size_t i = 0; i < vertices_data.size(); i++) 
                vertices_data[i].color_vec = {p_sprite_info->color.first, p_sprite_info->color.second, p_sprite_info->color.third, p_sprite_info->color.fourth};

            indices_data = {0, 1, 2, 2, 3, 0};
        }
    }

    RectangleSprite::RectangleSprite(RectangleSpriteInfo *p_rectangle_spriteinfo) {
        VerticesDataCreator::getRectangleVertices(p_rectangle_spriteinfo, this->m_vertices_data, this->m_indices_data);
        this->m_origin = p_rectangle_spriteinfo->origin;
        this->m_position = p_rectangle_spriteinfo->position;
        this->m_size = p_rectangle_spriteinfo->size;
        this->m_color = p_rectangle_spriteinfo->color;
    }

    void RectangleSprite::reloadVertices() {
        RectangleSpriteInfo local_spriteinfo{};
        local_spriteinfo.color = this->m_color;
        local_spriteinfo.origin = this->m_origin;
        local_spriteinfo.position = this->m_position;
        local_spriteinfo.size = this->m_size;

        VerticesDataCreator::getRectangleVertices(&local_spriteinfo, this->m_vertices_data, this->m_indices_data);
    }

    void RectangleSprite::updateOrigin(const deng::vec2<float> &new_origin) {
        this->m_origin = new_origin;
        RectangleSpriteInfo local_spriteinfo{};
        local_spriteinfo.color = this->m_color;
        local_spriteinfo.origin = this->m_origin;
        local_spriteinfo.position = this->m_position;
        local_spriteinfo.size = this->m_size;

        VerticesDataCreator::getRectangleVertices(&local_spriteinfo, this->m_vertices_data, this->m_indices_data);
    }

    void RectangleSprite::updatePosition(const deng::vec2<float> &new_position) {
        this->m_position = new_position;
        RectangleSpriteInfo local_spriteinfo{};
    }

    void RectangleSprite::getVertices(std::vector<UIVerticesData> *vertices_data, std::vector<uint16_t> *indices_data) {
        *vertices_data = this->m_vertices_data;
        *indices_data = this->m_indices_data;
    }
}