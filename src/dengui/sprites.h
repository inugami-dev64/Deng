namespace dengUI {

    struct UIVerticesData {
        deng::vec2<float> position_vec;
        deng::vec4<float> color_vec;
    };

    struct RectangleSpriteInfo {
        deng::vec2<float> size;
        deng::vec4<float> color;
        deng::vec2<float> origin = {0, 0};
        deng::vec2<float> position;
    };

    struct VerticesDataCreator {
        static void getRectangleVertices(RectangleSpriteInfo *p_sprite_info, std::vector<UIVerticesData> &vertices_data, std::vector<uint16_t> &indices_data);
    };

    class RectangleSprite {
    private:
        std::vector<uint16_t> m_indices_data; 
        std::vector<UIVerticesData> m_vertices_data;
        
        deng::vec4<float> m_color;
        deng::vec2<float> m_origin = {0, 0};
        deng::vec2<float> m_size;
        deng::vec2<float> m_position;

    private:
        void reloadVertices();

    public:
        RectangleSprite(RectangleSpriteInfo *p_rectangle_spriteinfo);
        void updateOrigin(const deng::vec2<float> &new_origin);
        void updatePosition(const deng::vec2<float> &new_position);
        void getVertices(std::vector<UIVerticesData> *vertices_data, std::vector<uint16_t> *indices_data);

    };
}  