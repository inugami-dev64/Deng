/*    ____         ________     __      _        ______  
 *   |  _ \_      |  ______|   |  \    | |     _/  ____\
 *   | | \_ \_    |  |         |   \   | |    /  _/   
 *   | |   \  |   |  |_____    | |\ \  | |   |  |   _____ 
 *   | |    | |   |  ______|   | | \ \ | |   |  |  |___  | 
 *   | |  _/ _|   |  |         | |  \ \| |    \  \    / /
 *   | |_/ _/     |  |_____    | |   \   |     \_ \__/ /
 *   |____/       |________|   |_|    \__|       \____/
 *               
 * __________________________________________________________
 * __________________________________________________________
 *                       Project DENG 
 *
 *
 * Copyright (C) 2020 - 2021
 * This Software is licensed under Apache License as described 
 * in the LICENSE.md file, which you should have recieved with 
 * this distribution.
 * 
 * You may reproduce and distribute copies of the
 * Work or Derivative Works thereof in any medium, with or without
 * modifications, and in Source or Object form, provided that You
 * meet the following conditions:
 *
 * (a) You must give any other recipients of the Work or
 *     Derivative Works a copy of this License; and
 *
 * (b) You must cause any modified files to carry prominent notices
 *     stating that You changed the files; and
 *
 * (c) You must retain, in the Source form of any Derivative Works
 *     that You distribute, all copyright, patent, trademark, and
 *     attribution notices from the Source form of the Work,
 *     excluding those notices that do not pertain to any part of
 *     the Derivative Works; and
 *
 * (d) If the Work includes a "NOTICE" text file as part of its
 *     distribution, then any Derivative Works that You distribute must
 *     include a readable copy of the attribution notices contained
 *     within such NOTICE file, excluding those notices that do not
 *     pertain to any part of the Derivative Works, in at least one
 *     of the following places: within a NOTICE text file distributed
 *     as part of the Derivative Works; within the Source form or
 *     documentation, if provided along with the Derivative Works; or,
 *     within a display generated by the Derivative Works, if and
 *     wherever such third-party notices normally appear. The contents
 *     of the NOTICE file are for informational purposes only and
 *     do not modify the License. You may add Your own attribution
 *     notices within Derivative Works that You distribute, alongside
 *     or as an addendum to the NOTICE text from the Work, provided
 *     that such additional attribution notices cannot be construed
 *     as modifying the License.
 *
 * You may add Your own copyright statement to Your modifications and
 * may provide additional or different license terms and conditions
 * for use, reproduction, or distribution of Your modifications, or
 * for any such Derivative Works as a whole, provided Your use,
 * reproduction, and distribution of the Work otherwise complies with
 * the conditions stated in this License.
 */ 


#define __SHAPE_GEN_CPP
#include <utils/shape_gen.h>

namespace dengUtils {

    /* 
     * Generate border for generic unmapped vertices 
     */
    void BorderGenerator::generateBorders (
        std::vector<VERT_UNMAPPED_2D> &vert,
        deng_bool_t new_vert,
        deng_ui64_t vert_offset,
        std::vector<deng_ui32_t> &indices,
        dengMath::vec4<deng_vec_t> color,
        dengMath::vec2<deng_ui32_t> deng_window_size,
        deng_ui32_t hier_level,
        deng_px_t border_thickness
    ) {
        if(!border_thickness) return;

        // Convert border size from pixel size to vector 2D size
        deng_vec_t vec_border_width = dengMath::Conversion::pixelSizeToVector2DSize (
            border_thickness,
            deng_window_size.first
        );

        std::vector<VERT_UNMAPPED_2D> border_verts(2 * (vert.size() - vert_offset));
        std::vector<deng_ui32_t> border_indices(6 * (vert.size() - vert_offset));

        deng_i64_t c_index, p_index, n_index, push_index;
        dengMath::vec2<deng_vec_t> a_vec, b_vec, norm_vec;
        
        // Find all border vertices
        for(c_index = vert_offset, push_index = 0; c_index < (deng_i64_t) vert.size(); c_index++, push_index += 2) {
            p_index = c_index - 1;
            n_index = c_index + 1;
            if(p_index < (deng_i64_t) vert_offset) 
                p_index = (deng_i64_t) vert.size() - 1;
            if(n_index == (deng_i64_t) vert.size()) 
                n_index = (deng_i64_t) vert_offset;

            // Find all constructed triangle side vectors
            a_vec.first = vert[c_index].vert_data.vert_x - vert[p_index].vert_data.vert_x;
            a_vec.second = vert[c_index].vert_data.vert_y - vert[p_index].vert_data.vert_y;

            b_vec.first = vert[n_index].vert_data.vert_x - vert[c_index].vert_data.vert_x;
            b_vec.second = vert[n_index].vert_data.vert_y - vert[c_index].vert_data.vert_y;

            deng_vec_t border_sine, border_cosine;

            // Make b vector magnitude the same as a vector
            deng_vec_t mag_a = (deng_vec_t) sqrtf (
                (a_vec.first * a_vec.first) +
                (a_vec.second * a_vec.second)
            );

            deng_vec_t sin_b = b_vec.second / sqrtf (
                (b_vec.first * b_vec.first) +
                (b_vec.second * b_vec.second)
            );

            deng_vec_t cos_b = b_vec.first / sqrt (
                (b_vec.first * b_vec.first) +
                (b_vec.second * b_vec.second)
            );

            b_vec.first = mag_a * cos_b;
            b_vec.second = mag_a * sin_b;

            norm_vec = a_vec + b_vec;

            border_sine = norm_vec.second / sqrtf (
                (norm_vec.first * norm_vec.first) +
                (norm_vec.second * norm_vec.second)
            );

            border_cosine = -norm_vec.first / sqrtf (
                norm_vec.first * norm_vec.first +
                norm_vec.second * norm_vec.second
            );

            border_verts[push_index].vert_data = vert[c_index].vert_data;

            border_verts[push_index + 1].vert_data.vert_x = border_sine * vec_border_width + vert[c_index].vert_data.vert_x;
            border_verts[push_index + 1].vert_data.vert_y = border_cosine * vec_border_width + vert[c_index].vert_data.vert_y;
        }


        // Index all vertices
        for(c_index = 0, push_index = 0; c_index < (deng_vec_t) border_verts.size(); c_index += 2, push_index += 6) {
            p_index = c_index - 1;
            n_index = c_index + 2;
            if(p_index < 0) 
                p_index = (deng_i64_t) border_verts.size() - 1;
            if(n_index >= (deng_i64_t) border_verts.size()) 
                n_index = 0;

            border_indices[push_index] = (deng_ui32_t) (c_index + 1) + (deng_ui32_t) (vert.size() - vert_offset);
            border_indices[push_index + 1] = (deng_ui32_t) n_index + (deng_ui32_t) (vert.size() - vert_offset);
            border_indices[push_index + 2] = (deng_ui32_t) c_index + (deng_ui32_t) (vert.size() - vert_offset);

            border_indices[push_index + 3] = (deng_ui32_t) (c_index + 1) + (deng_ui32_t) (vert.size() - vert_offset);
            border_indices[push_index + 4] = (deng_ui32_t) p_index + (deng_ui32_t) (vert.size() - vert_offset);
            border_indices[push_index + 5] = (deng_ui32_t) c_index + (deng_ui32_t) (vert.size() - vert_offset);
        }

        if(new_vert) vert.clear();

        // Insert newly generated vertices and indices into output vectors
        vert.insert (
            vert.end(),
            border_verts.begin(),
            border_verts.end()
        ); 

        indices.insert (
            indices.end(),
            border_indices.begin(),
            border_indices.end()
        );
    }

    
    /* 
     * Create unmapped 2d asset from given vertices and indices
     */
    das_Asset AssetMaker2D::makeUnmappedAsset (
        const char *asset_uuid,
        const char *asset_name,
        std::vector<VERT_UNMAPPED_2D> &vert,
        std::vector<deng_ui32_t> &indices
    ) {
        das_Asset out_asset;
        out_asset.asset_mode = DAS_ASSET_MODE_2D_UNMAPPED;

        // Copy asset name over
        out_asset.name = (char*) calloc (
            strlen(asset_name) + 1,
            sizeof(char)
        );
        strcpy(out_asset.name, asset_name);

        // Copy asset uuid over
        out_asset.uuid = (char*) calloc (
            strlen(asset_uuid) + 1,
            sizeof(char)
        );
        strcpy(out_asset.uuid, asset_uuid);
        
        // Allocate memory and copy indices over
        out_asset.indices.n = indices.size();
        out_asset.indices.indices = (deng_ui32_t*) calloc (
            indices.size(),
            sizeof(deng_ui32_t)
        );

        memcpy (
            out_asset.indices.indices,
            indices.data(),
            indices.size() * sizeof(deng_ui32_t)
        );

        // Allocate memory and copy all vertices over
        out_asset.vertices.n = vert.size();
        out_asset.vertices.vu2d = (VERT_UNMAPPED_2D*) calloc (
            vert.size(),
            sizeof(VERT_UNMAPPED_2D)
        );
        
        memcpy (
            out_asset.vertices.vu2d,
            vert.data(),
            vert.size() * sizeof(VERT_UNMAPPED_2D)
        );

        return out_asset;
    }


    /* 
     * Create texture mapped 2d asset from given vertices and indices
     */
    das_Asset AssetMaker2D::makeTexMappedAsset (
        const char *asset_uuid,
        const char *asset_name,
        const char *tex_uuid,
        std::vector<VERT_MAPPED_2D> &vert,
        std::vector<deng_ui32_t> &indices
    ) {
        das_Asset out_asset;
        out_asset.asset_mode = DAS_ASSET_MODE_2D_TEXTURE_MAPPED;

        // Copy uuid over
        out_asset.uuid = (char*) calloc (
            strlen(asset_uuid) + 1,
            sizeof(char)
        );
        strcpy(out_asset.uuid, asset_uuid);

        // Copy asset name over
        out_asset.name = (char*) calloc (
            strlen(asset_name) + 1,
            sizeof(char)
        );

        // Copy texture uuid over
        out_asset.tex_uuid = (char*) calloc (
            strlen(tex_uuid) + 1,
            sizeof(char) 
        );
        strcpy(out_asset.tex_uuid, tex_uuid);

        // Allocate memory and copy indices over
        out_asset.indices.n = indices.size();
        out_asset.indices.indices = (deng_ui32_t*) calloc (
            indices.size(),
            sizeof(deng_ui32_t)
        );

        memcpy (
            out_asset.indices.indices,
            indices.data(),
            indices.size() * sizeof(deng_ui32_t)
        );

        // Allocate memory and copy vertices over
        out_asset.vertices.n = vert.size();
        out_asset.vertices.vm2d = (VERT_MAPPED_2D*) calloc (
            vert.size(),
            sizeof(VERT_MAPPED_2D)
        );

        memcpy (
            out_asset.vertices.vm2d,
            vert.data(),
            vert.size() * sizeof(VERT_MAPPED_2D)
        );

        return out_asset;
    }

    RectangleGenerator::RectangleGenerator (
        const dengMath::vec2<deng_ui32_t> &draw_bounds
    ) { m_draw_bounds = (dengMath::vec2<deng_ui32_t>) draw_bounds; }


    /* 
     * Generate unmapped vertices with absolute position 
     */
    void RectangleGenerator::generateUnmappedAbsRec (
        const dengMath::vec2<deng_vec_t> &pos,
        const dengMath::vec2<deng_vec_t> &size,
        const dengMath::vec2<deng_vec_t> &origin,
        const dengMath::vec4<deng_vec_t> &color,
        const deng_ui32_t hier_level,
        std::vector<VERT_UNMAPPED_2D> &vert
    ) {
        deng_ui64_t vert_offset = vert.size();
        vert.resize(vert_offset + 4);
        
        vert[vert_offset].vert_data.vert_x = pos.first - size.first * ((origin.first + 1.0f) / 2);
        vert[vert_offset].vert_data.vert_y = pos.second - size.second * ((origin.second + 1.0f) / 2);

        vert[vert_offset + 1].vert_data.vert_x = pos.first + size.first * (1.0f - (origin.first + 1.0f) / 2);
        vert[vert_offset + 1].vert_data.vert_y = pos.second - size.second * ((origin.second + 1.0f) / 2);

        vert[vert_offset + 2].vert_data.vert_x = pos.first + size.first * (1.0f - (origin.first + 1.0f) / 2);
        vert[vert_offset + 2].vert_data.vert_y = pos.second + size.second * (1.0f - (origin.second + 1.0f) / 2);

        vert[vert_offset + 3].vert_data.vert_x = pos.first - size.first * ((origin.first + 1.0f) / 2);
        vert[vert_offset + 3].vert_data.vert_y = pos.second + size.second * (1.0f - (origin.second + 1.0f) / 2);
    }


    /* 
     * Generate unmapped vertices with relative position 
     */
    void RectangleGenerator::generateUnmappedRelRec (
        const dengMath::vec2<deng_vec_t> &pos,
        dengMath::vec2<deng_vec_t> size,
        deng_bool_t is_abs_size,
        const dengMath::vec2<deng_vec_t> &origin,
        VERT_UNMAPPED_2D *outer_rec,
        const dengMath::vec4<deng_vec_t> &color,
        deng_ui32_t hier_level,
        std::vector<VERT_UNMAPPED_2D> &vert  
    ) {
        // Find the outer rectangle size
        dengMath::vec2<deng_vec_t> outer_rec_size;
        outer_rec_size.first = outer_rec[1].vert_data.vert_x - outer_rec[0].vert_data.vert_x;
        outer_rec_size.second = outer_rec[3].vert_data.vert_y - outer_rec[0].vert_data.vert_y;

        // Calculate the absolute position of the relative rectangle
        dengMath::vec2<deng_vec_t> abs_pos;
        abs_pos.first = (pos.first + 1.0f) * outer_rec_size.first / 2 + outer_rec[0].vert_data.vert_x;
        abs_pos.second = (pos.second + 1.0f) * outer_rec_size.second / 2 + outer_rec[0].vert_data.vert_y;
        
        if(!is_abs_size) {
            size.first = size.first * outer_rec_size.first / 2;
            size.second = size.second * outer_rec_size.second / 2;
        }

        RectangleGenerator::generateUnmappedAbsRec (
            abs_pos,
            size,
            origin,
            color,
            hier_level,
            vert
        );
    }


    /* 
     * Generate texture mapped vertices with absolute position 
     */
    void RectangleGenerator::generateMappedAbsRec (
        const dengMath::vec2<deng_vec_t> &pos,
        const dengMath::vec2<deng_vec_t> &size,
        const dengMath::vec2<deng_vec_t> &origin,
        deng_ui32_t hier_level,
        std::vector<VERT_MAPPED_2D> &vert
    ) {
        deng_ui64_t vert_offset = vert.size();
        vert.resize(vert_offset + 4);

        vert[vert_offset].vert_data.vert_x = pos.first - size.first * ((origin.first + 1.0f) / 2);
        vert[vert_offset].vert_data.vert_y = pos.second - size.second * ((origin.second + 1.0f) / 2);
        vert[vert_offset].tex_data = {0.0f, 0.0f};

        vert[vert_offset + 1].vert_data.vert_x = pos.first + size.first * (1.0f - (origin.first + 1.0f) / 2);
        vert[vert_offset + 1].vert_data.vert_y = pos.second - size.second * ((origin.second + 1.0f) / 2);
        vert[vert_offset + 1].tex_data = {1.0f, 0.0f};

        vert[vert_offset + 2].vert_data.vert_x = pos.first + size.first * (1.0f - (origin.first + 1.0f) / 2);
        vert[vert_offset + 2].vert_data.vert_y = pos.second + size.second * (1.0f - (origin.second + 1.0f) / 2);
        vert[vert_offset + 2].tex_data= {1.0f, 1.0f};

        vert[vert_offset + 3].vert_data.vert_x = pos.first - size.first * ((origin.first + 1.0f) / 2);
        vert[vert_offset + 3].vert_data.vert_y = pos.second + size.second * (1.0f - (origin.second + 1.0f) / 2);
        vert[vert_offset + 3].tex_data = {0.0f, 1.0f};
    }


    /* 
     * Generate texture mapped vertices with relative position 
     */
    void RectangleGenerator::generateMappedRelRec (
        const dengMath::vec2<deng_vec_t> &pos,
        dengMath::vec2<deng_vec_t> size,
        deng_bool_t is_abs_size,
        const dengMath::vec2<deng_vec_t> &origin,
        VERT_UNMAPPED_2D *outer_rec,
        deng_ui32_t hier_level,
        std::vector<VERT_MAPPED_2D> &vert
    ) {
        dengMath::vec2<deng_vec_t> outer_rec_size;
        outer_rec_size.first = outer_rec[1].vert_data.vert_x - outer_rec[0].vert_data.vert_x;
        outer_rec_size.second = outer_rec[3].vert_data.vert_y - outer_rec[0].vert_data.vert_y;

        dengMath::vec2<deng_vec_t> abs_pos;
        abs_pos.first = (pos.first + 1.0f) * outer_rec_size.first / 2 + outer_rec[0].vert_data.vert_x;
        abs_pos.second = (pos.second + 1.0f) * outer_rec_size.second / 2 + outer_rec[0].vert_data.vert_y;
        
        if(!is_abs_size) {
            size.first = size.first * outer_rec_size.first / 2;
            size.second = size.second * outer_rec_size.second / 2;
        }

        RectangleGenerator::generateMappedAbsRec (
            abs_pos,
            size,
            origin,
            hier_level,
            vert
        );
    }


    /* Generate unmapped rectangle asset with absolute position */
    das_Asset RectangleGenerator::makeUnmappedAbsRecAsset (
        const char *asset_uuid,
        const char *asset_name,
        const dengMath::vec2<deng_vec_t> &pos,
        const dengMath::vec2<deng_vec_t> &size,
        const dengMath::vec2<deng_vec_t> &origin,
        const dengMath::vec4<deng_vec_t> &color,
        deng_ui32_t hier_level,
        deng_px_t border_width,
        const dengMath::vec4<deng_vec_t> &border_color
    ) {
        std::vector<VERT_UNMAPPED_2D> vert;
        std::vector<deng_ui32_t> indices = {0, 1, 2, 2, 3, 0};

        RectangleGenerator::generateUnmappedAbsRec (
            pos,
            size,
            origin,
            color,
            hier_level,
            vert
        );

        RectangleGenerator::BorderGenerator::generateBorders (
            vert,
            false,
            0,
            indices,
            border_color,
            m_draw_bounds,
            hier_level,
            border_width
        );

        return RectangleGenerator::AssetMaker2D::makeUnmappedAsset (
            asset_uuid,
            asset_name,
            vert,
            indices
        );
    }


    /* 
     * Generate texture mapped rectangle asset with absolute position 
     */
    das_Asset RectangleGenerator::makeMappedAbsRecAsset (
        const char *asset_uuid,
        const char *asset_name,
        const char *tex_uuid,
        const dengMath::vec2<deng_vec_t> &pos,
        const dengMath::vec2<deng_vec_t> &size,
        deng_ui32_t hier_level,
        const dengMath::vec2<deng_vec_t> &origin
    ) {
        std::vector<VERT_MAPPED_2D> vert;
        std::vector<deng_ui32_t> indices = {0, 1, 2, 2, 3, 0};
        RectangleGenerator::generateMappedAbsRec (
            pos,
            size,
            origin,
            hier_level,
            vert
        );

        return RectangleGenerator::AssetMaker2D::makeTexMappedAsset (
            asset_uuid,
            asset_name,
            tex_uuid,
            vert,
            indices
        );
    }


    /* 
     * Generate unmapped recatngle asset with relative position 
     */
    das_Asset RectangleGenerator::makeUnmappedRelRecAsset (
        const char *asset_uuid,
        const char *asset_name,
        const dengMath::vec2<deng_vec_t> &pos,
        const dengMath::vec2<deng_vec_t> &size,
        deng_bool_t is_abs_size,
        const dengMath::vec2<deng_vec_t> &origin,
        const dengMath::vec4<deng_vec_t> &color,
        VERT_UNMAPPED_2D *outer_rec,
        deng_px_t border_width,
        deng_ui32_t hier_level,
        const dengMath::vec4<deng_vec_t> &border_color
    ) {
        std::vector<VERT_UNMAPPED_2D> vert;
        std::vector<deng_ui32_t> indices;

        RectangleGenerator::generateUnmappedRelRec (
            pos,
            size,
            is_abs_size,     
            origin,
            outer_rec,
            color,
            hier_level,
            vert
        );

        RectangleGenerator::BorderGenerator::generateBorders (
            vert,
            false,
            0,
            indices,
            color,
            m_draw_bounds,
            hier_level,
            border_width
        );

        return RectangleGenerator::AssetMaker2D::makeUnmappedAsset (
            asset_uuid,
            asset_name,
            vert,
            indices
        );
    }


    /* 
     * Generate texture mapped rectangle asset with relative position 
     */
    das_Asset RectangleGenerator::makeMappedRelRecAsset (
        const char *asset_uuid,
        const char *asset_name,
        const char *tex_uuid,
        const dengMath::vec2<deng_vec_t> &pos,
        const dengMath::vec2<deng_vec_t> &size,
        deng_bool_t is_abs_size,
        const dengMath::vec2<deng_vec_t> &origin,
        const dengMath::vec4<deng_vec_t> &color,
        deng_ui32_t hier_level,
        VERT_UNMAPPED_2D *outer_rec
    ) {
        std::vector<VERT_MAPPED_2D> vert;
        std::vector<deng_ui32_t> indices = {0, 1, 2, 2, 3, 0};

        RectangleGenerator::generateMappedRelRec (
            pos,
            size,
            is_abs_size,
            origin,
            outer_rec,
            hier_level,
            vert
        );

        return RectangleGenerator::AssetMaker2D::makeTexMappedAsset (
            asset_uuid,
            asset_name,
            tex_uuid,
            vert,
            indices
        );
    }


    TriangleGenerator::TriangleGenerator (
        const dengMath::vec2<deng_ui32_t> &draw_bounds
    ) { m_draw_bounds = draw_bounds; }


    /* Generate triangle from absolute position */
    void TriangleGenerator::generateAbsTriangle (
        std::vector<VERT_UNMAPPED_2D> &vert,
        const dengMath::vec2<deng_vec_t> &tri_rec_pos,
        const dengMath::vec2<deng_vec_t> &tri_rec_size,
        const dengMath::vec2<deng_vec_t> &tri_rec_origin,
        const dengMath::vec4<deng_vec_t> &color,
        deng_ui32_t hier_level,
        const std::array<dengMath::vec2<deng_vec_t>, 3> &tri_rec_triangle_pos
    ) {
        deng_ui64_t vert_offset = vert.size();
        vert.resize(vert_offset + 3);
        std::array<das_ObjVertData2D, 4> tri_rec_vert;

        // Calculate all surrounding triangle vertices
        tri_rec_vert[0].vert_x = tri_rec_pos.first - ((tri_rec_origin.first + 1.0f) / 2 * tri_rec_size.first);
        tri_rec_vert[0].vert_y = tri_rec_pos.second - ((tri_rec_origin.second + 1.0f) / 2 * tri_rec_size.second);
        tri_rec_vert[1].vert_x = tri_rec_pos.first + ((1.0f - (tri_rec_origin.first + 1.0f) / 2) * tri_rec_size.first);
        tri_rec_vert[1].vert_y = tri_rec_pos.second - ((tri_rec_origin.first + 1.0f) / 2 * tri_rec_size.second);
        tri_rec_vert[2].vert_x = tri_rec_pos.first + ((1.0f - (tri_rec_origin.first + 1.0f) / 2) * tri_rec_size.first);
        tri_rec_vert[2].vert_y = tri_rec_pos.second + ((1.0f - (tri_rec_origin.second + 1.0f) / 2) * tri_rec_size.second);
        tri_rec_vert[3].vert_x = tri_rec_pos.first - ((tri_rec_origin.first + 1.0f) / 2 * tri_rec_size.first);
        tri_rec_vert[3].vert_y = tri_rec_pos.second + ((1.0f - (tri_rec_origin.second + 1.0f) / 2) * tri_rec_size.second);
    
        deng_vec_t tri_rec_width = tri_rec_vert[1].vert_x - tri_rec_vert[0].vert_x;
        deng_vec_t tri_rec_height = tri_rec_vert[3].vert_y - tri_rec_vert[0].vert_y;
        

        // All triangle vertices from its surrounding rectangle are taken from top - left corner
        vert[vert_offset].vert_data.vert_x = tri_rec_vert[0].vert_x + (tri_rec_triangle_pos[0].first + 1.0f) / 2 * tri_rec_width;
        vert[vert_offset].vert_data.vert_y = tri_rec_vert[0].vert_y + (tri_rec_triangle_pos[0].second + 1.0f) / 2 * tri_rec_height;

        vert[vert_offset + 1].vert_data.vert_x = tri_rec_vert[0].vert_x + (tri_rec_triangle_pos[1].first + 1.0f) / 2 * tri_rec_width;
        vert[vert_offset + 1].vert_data.vert_y = tri_rec_vert[0].vert_y + (tri_rec_triangle_pos[1].second + 1.0f) / 2 * tri_rec_height;

        vert[vert_offset + 2].vert_data.vert_x = tri_rec_vert[0].vert_x + (tri_rec_triangle_pos[2].first + 1.0f) / 2 * tri_rec_width;
        vert[vert_offset + 2].vert_data.vert_y = tri_rec_vert[0].vert_y + (tri_rec_triangle_pos[2].second + 1.0f) / 2 * tri_rec_height;
    }


    /* Generate triangle from relative position */
    void TriangleGenerator::generateRelTriangle (
        std::vector<VERT_UNMAPPED_2D> &vert,
        VERT_UNMAPPED_2D *outer_rec,
        dengMath::vec2<deng_vec_t> tri_rec_pos,
        dengMath::vec2<deng_vec_t> tri_rec_size,
        const dengMath::vec2<deng_vec_t> &tri_rec_origin,
        const dengMath::vec4<deng_vec_t> &color,
        deng_bool_t is_abs_size,
        deng_ui32_t hier_level,
        const std::array<dengMath::vec2<deng_vec_t>, 3> &tri_rec_triangle_pos 
    ) {
        dengMath::vec2<deng_vec_t> outer_rec_size;
        outer_rec_size.first = outer_rec[1].vert_data.vert_x - outer_rec[0].vert_data.vert_x;
        outer_rec_size.second = outer_rec[3].vert_data.vert_y - outer_rec[0].vert_data.vert_y;
        
        tri_rec_pos.first = (tri_rec_pos.first + 1.0f) / 2 * outer_rec_size.first + outer_rec[0].vert_data.vert_x;
        tri_rec_pos.second = (tri_rec_pos.second + 1.0f) / 2 * outer_rec_size.second + outer_rec[0].vert_data.vert_y;
        
        if(!is_abs_size) {
            tri_rec_size.first = tri_rec_size.first / 2 * outer_rec_size.first;
            tri_rec_size.second = tri_rec_size.second / 2 * outer_rec_size.second;
        }

        TriangleGenerator::generateAbsTriangle (
            vert,
            tri_rec_pos,
            tri_rec_size,
            tri_rec_origin,
            color,
            hier_level,
            tri_rec_triangle_pos
        );
    }


    /* 
     * Generate triangle shaped asset from absolute position 
     */
    das_Asset TriangleGenerator::makeAbsTriangleAsset (
        const char *asset_uuid,
        const char *asset_name,
        const dengMath::vec2<deng_vec_t> &tri_rec_pos,
        const dengMath::vec2<deng_vec_t> &tri_rec_size,
        const dengMath::vec2<deng_vec_t> &tri_rec_origin,
        const dengMath::vec4<deng_vec_t> &color,
        const std::array<dengMath::vec2<deng_vec_t>, 3> &tri_rec_triangle_pos,
        deng_px_t border_width,
        deng_ui32_t hier_level,
        const dengMath::vec4<deng_vec_t> &border_color
    ) {
        std::vector<VERT_UNMAPPED_2D> vert;
        std::vector<deng_ui32_t> indices = {0, 1, 2};

        TriangleGenerator::generateAbsTriangle (
            vert,
            tri_rec_pos,
            tri_rec_size,
            tri_rec_origin,
            color,
            hier_level,
            tri_rec_triangle_pos
        );

        TriangleGenerator::BorderGenerator::generateBorders (
            vert,
            false,
            0,
            indices,
            border_color,
            m_draw_bounds,
            hier_level,
            border_width
        );

        return TriangleGenerator::AssetMaker2D::makeUnmappedAsset (
            asset_uuid,
            asset_name,
            vert,
            indices
        );
    }


    /* Generate triangle shaped asset from relative position */
    das_Asset TriangleGenerator::makeRelTriangleAsset (
        const char *asset_uuid,
        const char *asset_name,
        VERT_UNMAPPED_2D *outer_rec,
        const dengMath::vec2<deng_vec_t> &tri_rec_pos,
        const dengMath::vec2<deng_vec_t> &tri_rec_size,
        const dengMath::vec2<deng_vec_t> &tri_rec_origin,
        const dengMath::vec4<deng_vec_t> &color,
        const std::array<dengMath::vec2<deng_vec_t>, 3> &tri_rec_triangle_pos,
        deng_px_t border_width,
        const dengMath::vec4<deng_vec_t> &border_color,
        deng_ui32_t hier_level,
        deng_bool_t is_abs_size
    ) {
        std::vector<VERT_UNMAPPED_2D> vert;
        std::vector<deng_ui32_t> indices = {0, 1, 2};

        TriangleGenerator::generateRelTriangle (
            vert,
            outer_rec,
            tri_rec_pos,
            tri_rec_size,
            tri_rec_origin,
            color,
            is_abs_size,
            hier_level,
            tri_rec_triangle_pos
        );

        TriangleGenerator::BorderGenerator::generateBorders (
            vert,
            false,
            0,
            indices,
            border_color,
            m_draw_bounds,
            hier_level,
            border_width
        );

        return TriangleGenerator::AssetMaker2D::makeUnmappedAsset (
            asset_uuid,
            asset_name,
            vert,
            indices
        );
    }


    CircleGenerator::CircleGenerator (
        const dengMath::vec2<deng_ui32_t> &draw_bounds
    ) { m_draw_bounds = draw_bounds; }


    /* Generate basic circle into vertices */
    void CircleGenerator::generateAbsCircle (
        std::vector<VERT_UNMAPPED_2D> &vert,
        std::vector<deng_ui32_t> &indices,
        const dengMath::vec2<deng_vec_t> &pos,
        deng_vec_t radius,
        deng_ui32_t hier_level,
        const dengMath::vec4<deng_vec_t> &color
    ) {
        // Find the total count of vertices for a circle
        deng_i32_t vert_c = (deng_i32_t) (
            radius *
            10 *
            DENG_CIRCLE_VERT_COUNT_MULTIPLIER
        );

        if(vert_c < DENG_CIRCLE_MIN_VERT_COUNT) 
            vert_c = DENG_CIRCLE_MIN_VERT_COUNT;

        deng_vec_t step = dengMath::Conversion::degToRad(360.0f / (deng_vec_t) vert_c);

        deng_i32_t l_index, n_index, i;
        std::vector<VERT_UNMAPPED_2D> cir_vert;
        cir_vert.resize(vert_c + 1);

        das_ObjVertData2D *p_pos = (das_ObjVertData2D*) &pos;

        // Center point
        cir_vert[0].vert_data = *p_pos;

        std::vector<deng_ui32_t> cir_indices;
        cir_indices.resize(vert_c * 3);

        deng_vec_t cur_step = 0;

        // Find all circle vertices
        for(l_index = 1; l_index < (deng_i32_t) cir_vert.size(); l_index++) {
            cur_step += step;
            cir_vert[l_index].vert_data.vert_x = radius * sin(cur_step) + pos.first;
            cir_vert[l_index].vert_data.vert_y = -radius * cos(cur_step) + pos.second;
        }

        // Find all circle indices
        for(l_index = 1, i = 0; l_index < (deng_i32_t) cir_vert.size(); l_index++, i += 3) {
            n_index = l_index + 1;
            if(n_index >= (deng_i32_t) cir_vert.size()) n_index = 1;
            cir_indices[i] = (deng_ui32_t) vert.size() + (deng_ui32_t) l_index;
            cir_indices[i + 1] = (deng_ui32_t) vert.size() + (deng_ui32_t) n_index;
            cir_indices[i + 2] = (deng_ui32_t) vert.size();
        } 

        // Insert new vertices and indices to output vectors
        vert.insert (
            vert.end(),
            cir_vert.begin(),
            cir_vert.end()
        );

        indices.insert (
            indices.end(),
            cir_indices.begin(),
            cir_indices.end()
        );
    }


    /* 
     * Generate circle with relative position 
     */
    void CircleGenerator::generateRelCircle (
        std::vector<VERT_UNMAPPED_2D> &vert,
        std::vector<deng_ui32_t> &indices,
        dengMath::vec2<deng_vec_t> pos,
        deng_vec_t radius,
        const dengMath::vec4<deng_vec_t> &color,
        deng_ui32_t hier_level,
        VERT_UNMAPPED_2D *outer_rec
    ) {
        deng_vec_t o_rec_width = outer_rec[1].vert_data.vert_x - outer_rec[0].vert_data.vert_x;
        deng_vec_t o_rec_height = outer_rec[3].vert_data.vert_y - outer_rec[0].vert_data.vert_y;

        pos.first = (pos.first + 1.0f) / 2 * o_rec_width + outer_rec[0].vert_data.vert_x;
        pos.second = (pos.second + 1.0f) / 2 * o_rec_height + outer_rec[0].vert_data.vert_y;
        radius = (radius + 1.0f) * o_rec_height * o_rec_width / 4;

        CircleGenerator::generateAbsCircle (
            vert,
            indices,
            pos,
            radius,
            hier_level,
            color
        );
    }


    /* 
     * Generate circle asset from absolute sizes 
     */
    das_Asset CircleGenerator::makeAbsCircleAsset (
        const char *asset_uuid,
        const char *asset_name,
        const dengMath::vec2<deng_vec_t> &pos,
        deng_vec_t radius,
        const dengMath::vec4<deng_vec_t> &color,
        deng_px_t border_width,
        deng_ui32_t hier_level,
        const dengMath::vec4<deng_vec_t> &border_color 
    ) {
        std::vector<VERT_UNMAPPED_2D> vertices;
        std::vector<deng_ui32_t> indices; 
        CircleGenerator::generateAbsCircle (
            vertices,
            indices,
            pos,
            radius,
            hier_level,
            color
        );

        CircleGenerator::BorderGenerator::generateBorders (
            vertices,
            false,
            1,
            indices,
            border_color,
            m_draw_bounds,
            hier_level,
            border_width
        );

        return AssetMaker2D::makeUnmappedAsset (
            asset_uuid,
            asset_name,
            vertices,
            indices
        );
    }


    /* Generate circle assets from relative sizes                               *
     * Asset will be calculated from top left vertex for surrounding rectangle  */
    das_Asset CircleGenerator::makeRelCircleAsset (
        const char *asset_uuid,
        const char *asset_name,
        const dengMath::vec2<deng_vec_t> &pos,
        deng_vec_t radius,
        const dengMath::vec4<deng_vec_t> &color,
        VERT_UNMAPPED_2D *surround_rec,
        deng_px_t border_width,
        deng_ui32_t hier_level,
        const dengMath::vec4<deng_vec_t> &border_color
    ) {
        std::vector<VERT_UNMAPPED_2D> vert;
        std::vector<deng_ui32_t> indices;
        
        CircleGenerator::generateRelCircle (
            vert,
            indices,
            pos,
            radius,
            color,
            hier_level,
            surround_rec
        );

        CircleGenerator::BorderGenerator::generateBorders (
            vert,
            false,
            1,
            indices,
            border_color,
            m_draw_bounds,
            hier_level,
            border_width
        );

        return CircleGenerator::AssetMaker2D::makeUnmappedAsset (
            asset_uuid,
            asset_name,
            vert,
            indices
        );
    }
}