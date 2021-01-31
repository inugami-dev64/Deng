#ifndef SHAPE_REC_H
#define SHAPE_REC_H
// Vertices per 0.1
#define DENG_CIRCLE_VERT_COUNT_MULTIPLIER   24
#define DENG_CIRCLE_MIN_VERT_COUNT          12

namespace dengUtils {

    /* Add borders around the shape */
    struct BorderGenerator {
        static void generateBorders (
            std::vector<VERT_UNMAPPED_2D> &vert,
            deng_bool_t new_vert,
            deng_ui64_t vert_offset,
            std::vector<deng_ui32_t> &indices,
            dengMath::vec4<deng_vec_t> color,
            dengMath::vec2<deng_ui32_t> deng_window_size,
            deng_px_t border_thickness 
        );
    };


    /* 2D asset construction */ 
    struct AssetMaker2D {
        static deng_Asset makeUnmappedAsset (
            const char *asset_id,
            const char *asset_desc,
            std::vector<VERT_UNMAPPED_2D> &vert,
            std::vector<deng_ui32_t> &indices
        );

        static deng_Asset makeTexMappedAsset (
            const char *asset_id,
            const char *tex_id,
            const char *asset_desc,
            std::vector<VERT_MAPPED_2D> &vert,
            std::vector<deng_ui32_t> &indices
        );
    };


    /* Collection of methods for generating rectangles */
    class RectangleGenerator : private BorderGenerator, AssetMaker2D {
    private:
        dengMath::vec2<deng_ui32_t> m_draw_bounds;

    public:
        RectangleGenerator(dengMath::vec2<deng_ui32_t> draw_bounds);
        static void generateUnmappedAbsRec (
            dengMath::vec2<deng_vec_t> pos,
            dengMath::vec2<deng_vec_t> size,
            dengMath::vec2<deng_vec_t> origin,
            dengMath::vec4<deng_vec_t> color,
            std::vector<VERT_UNMAPPED_2D> &vert   
        );

        static void generateUnmappedRelRec (
            dengMath::vec2<deng_vec_t> pos,
            dengMath::vec2<deng_vec_t> size,
            deng_bool_t is_abs_size,
            dengMath::vec2<deng_vec_t> origin,
            VERT_UNMAPPED_2D *outer_rec,
            dengMath::vec4<deng_vec_t> color,
            std::vector<VERT_UNMAPPED_2D> &vert
        );

        static void generateMappedAbsRec (
            dengMath::vec2<deng_vec_t> pos,
            dengMath::vec2<deng_vec_t> size,
            dengMath::vec2<deng_vec_t> origin,
            std::vector<VERT_MAPPED_2D> &vert
        );

        static void generateMappedRelRec (
            dengMath::vec2<deng_vec_t> pos,
            dengMath::vec2<deng_vec_t> size,
            deng_bool_t is_abs_size,
            dengMath::vec2<deng_vec_t> origin,
            VERT_UNMAPPED_2D *outer_rec,
            std::vector<VERT_MAPPED_2D> &vert
        );

        deng_Asset makeUnmappedAbsRecAsset (
            const char *asset_id,
            const char *asset_desc,
            dengMath::vec2<deng_vec_t> pos,
            dengMath::vec2<deng_vec_t> size,
            dengMath::vec2<deng_vec_t> origin,
            dengMath::vec4<deng_vec_t> color,
            deng_px_t border_width,
            dengMath::vec4<deng_vec_t> border_color
        );

        deng_Asset makeMappedAbsRecAsset (
            const char *asset_id,
            const char *tex_id,
            const char *asset_desc,
            dengMath::vec2<deng_vec_t> pos,
            dengMath::vec2<deng_vec_t> size,
            dengMath::vec2<deng_vec_t> origin
        );

        deng_Asset makeUnmappedRelRecAsset (
            const char *asset_id,
            const char *asset_desc,
            dengMath::vec2<deng_vec_t> pos,
            dengMath::vec2<deng_vec_t> size,
            deng_bool_t is_abs_size,
            dengMath::vec2<deng_vec_t> origin,
            dengMath::vec4<deng_vec_t> color,
            VERT_UNMAPPED_2D *outer_rec,
            deng_px_t border_width,
            dengMath::vec4<deng_vec_t> border_color
        );

        deng_Asset makeMappedRelRecAsset (
            const char *asset_id,
            const char *tex_id,
            const char *asset_desc,
            dengMath::vec2<deng_vec_t> pos,
            dengMath::vec2<deng_vec_t> size,
            deng_bool_t is_abs_size,
            dengMath::vec2<deng_vec_t> origin,
            dengMath::vec4<deng_vec_t> color,
            VERT_UNMAPPED_2D *outer_rec
        );
    };


    class TriangleGenerator : private BorderGenerator, private AssetMaker2D {
    private:
        dengMath::vec2<deng_ui32_t> m_draw_bounds;

    public:
        TriangleGenerator(dengMath::vec2<deng_ui32_t> draw_bounds);
        static void generateAbsTriangle (
            std::vector<VERT_UNMAPPED_2D> &vert,
            dengMath::vec2<deng_vec_t> tri_rec_pos,
            dengMath::vec2<deng_vec_t> tri_rec_size,
            dengMath::vec2<deng_vec_t> tri_rec_origin,
            dengMath::vec4<deng_vec_t> color,
            std::vector<dengMath::vec2<deng_vec_t>> tri_rec_triangle_pos 
        );

        static void generateRelTriangle (
            std::vector<VERT_UNMAPPED_2D> &vert,
            VERT_UNMAPPED_2D *outer_rec,  
            dengMath::vec2<deng_vec_t> tri_rec_pos,
            dengMath::vec2<deng_vec_t> tri_rec_size,
            dengMath::vec2<deng_vec_t> tri_rec_origin,
            dengMath::vec4<deng_vec_t> color,
            deng_bool_t is_abs_size,
            std::vector<dengMath::vec2<deng_vec_t>> tri_rec_triangle_pos
        );

        deng_Asset makeAbsTriangleAsset (
            const char *asset_id,
            const char *asset_desc,
            dengMath::vec2<deng_vec_t> tri_rec_pos,
            dengMath::vec2<deng_vec_t> tri_rec_size,
            dengMath::vec2<deng_vec_t> tri_rec_origin,
            dengMath::vec4<deng_vec_t> color, 
            std::vector<dengMath::vec2<deng_vec_t>> tri_rec_triangle_pos,
            deng_px_t border_width,
            dengMath::vec4<deng_vec_t> border_color
        );

        deng_Asset makeRelTriangleAsset (
            const char *asset_id,
            const char *asset_desc,
            VERT_UNMAPPED_2D *outer_rec,
            dengMath::vec2<deng_vec_t> tri_rec_pos,
            dengMath::vec2<deng_vec_t> tri_rec_size,
            dengMath::vec2<deng_vec_t> tri_rec_origin,
            dengMath::vec4<deng_vec_t> color,
            std::vector<dengMath::vec2<deng_vec_t>> tri_rec_triangle_pos,
            deng_px_t border_width,
            dengMath::vec4<deng_vec_t> border_color,
            deng_bool_t is_abs_size
        );
    };


    /* Circle generator */
    class CircleGenerator : private BorderGenerator, private AssetMaker2D {
    private:
        dengMath::vec2<deng_ui32_t> m_draw_bounds;

    public:
        CircleGenerator(dengMath::vec2<deng_ui32_t> draw_bounds);
        static void generateAbsCircle (
            std::vector<VERT_UNMAPPED_2D> &vert,
            std::vector<deng_ui32_t> &indices,
            dengMath::vec2<deng_vec_t> pos,
            deng_vec_t radius,
            dengMath::vec4<deng_vec_t> color
        );

        static void generateRelCircle (
            std::vector<VERT_UNMAPPED_2D> &vert,
            std::vector<deng_ui32_t> &indices,
            dengMath::vec2<deng_vec_t> pos,
            deng_vec_t radius,
            dengMath::vec4<deng_vec_t> color,
            VERT_UNMAPPED_2D *outer_rec
        );

        deng_Asset makeAbsCircleAsset (
            const char *asset_id,
            const char *asset_desc,
            dengMath::vec2<deng_vec_t> pos,
            deng_vec_t radius,
            dengMath::vec4<deng_vec_t> color,
            deng_px_t border_width,
            dengMath::vec4<deng_vec_t> border_color
        );

        deng_Asset makeRelCircleAsset (
            const char *asset_id,
            const char *asset_desc,
            dengMath::vec2<deng_vec_t> pos,
            deng_vec_t radius,
            dengMath::vec4<deng_vec_t> color,
            VERT_UNMAPPED_2D *surround_rec,
            deng_px_t border_width,
            dengMath::vec4<deng_vec_t> border_color
        );
    };
}

#endif
