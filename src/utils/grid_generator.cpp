#include "../../headers/deng/api_core.h"

namespace dengUtils {
    GridGenerator::GridGenerator (
        dengMath::vec4<deng_vec_t> color,
        deng_vec_t len,
        deng_ui32_t row_c
    ) {
        m_color = color;
        m_grid_len = len;
        m_row_c = row_c;
        
        deng_vec_t step = len / (deng_vec_t) row_c;

        // Setup asset
        m_asset.asset_mode = DENG_ASSET_MODE_3D_UNMAPPED;
        m_asset.id = (char*) "$GRID";
        m_asset.description = (char*) "$GRID";
        m_asset.vertices.size = row_c * 6;
        m_asset.vertices.p_unmapped_unnormalized_vert = (VERT_UNMAPPED_UNOR*) calloc (
            m_asset.vertices.size,
            sizeof(VERT_UNMAPPED_UNOR)
        );
        m_asset.indices.size = m_asset.vertices.size;
        m_asset.indices.p_indices = (deng_ui32_t*) calloc (
            m_asset.indices.size,
            sizeof(deng_ui32_t)
        );
        
        deng_vec_t cur_pos;
        deng_ObjColorData tmp_col;
        tmp_col.col_r = m_color.first;
        tmp_col.col_g = m_color.second;
        tmp_col.col_b = m_color.third;
        tmp_col.col_a = m_color.fourth;

        size_t l_index, r_index;

        // Z axis rays
        cur_pos = -(len / 2);
        for(l_index = 0; l_index < row_c * 3; l_index += 3) {
            m_asset.vertices.p_unmapped_unnormalized_vert[l_index].vert_data.vert_x = -(len / 2);
            m_asset.vertices.p_unmapped_unnormalized_vert[l_index].vert_data.vert_y = 0;
            m_asset.vertices.p_unmapped_unnormalized_vert[l_index].vert_data.vert_z = cur_pos;
            m_asset.vertices.p_unmapped_unnormalized_vert[l_index].color_data = tmp_col;
            
            m_asset.vertices.p_unmapped_unnormalized_vert[l_index + 1].vert_data.vert_x = len / 2;
            m_asset.vertices.p_unmapped_unnormalized_vert[l_index + 1].vert_data.vert_y = 0;
            m_asset.vertices.p_unmapped_unnormalized_vert[l_index + 1].vert_data.vert_z = cur_pos;
            m_asset.vertices.p_unmapped_unnormalized_vert[l_index + 1].color_data = tmp_col;

            m_asset.vertices.p_unmapped_unnormalized_vert[l_index + 2].vert_data.vert_x = -(len / 2);
            m_asset.vertices.p_unmapped_unnormalized_vert[l_index + 2].vert_data.vert_y = 0;
            m_asset.vertices.p_unmapped_unnormalized_vert[l_index + 2].vert_data.vert_z = cur_pos + DENG_GRID_THICKNESS;
            m_asset.vertices.p_unmapped_unnormalized_vert[l_index + 2].color_data = tmp_col;
            
            m_asset.indices.p_indices[l_index] = l_index;
            m_asset.indices.p_indices[l_index + 1] = l_index + 1;
            m_asset.indices.p_indices[l_index + 2] = l_index + 2;
            cur_pos += step;
        }

        // X axis rays
        r_index = l_index;
        cur_pos = -(len / 2);
        for(l_index = 0; l_index < row_c * 3; l_index += 3, r_index += 3) {
            m_asset.vertices.p_unmapped_unnormalized_vert[r_index].vert_data.vert_x = cur_pos;
            m_asset.vertices.p_unmapped_unnormalized_vert[r_index].vert_data.vert_y = 0;
            m_asset.vertices.p_unmapped_unnormalized_vert[r_index].vert_data.vert_z = -(len / 2);
            m_asset.vertices.p_unmapped_unnormalized_vert[r_index].color_data = tmp_col;

            m_asset.vertices.p_unmapped_unnormalized_vert[r_index + 1].vert_data.vert_x = cur_pos;
            m_asset.vertices.p_unmapped_unnormalized_vert[r_index + 1].vert_data.vert_y = 0;
            m_asset.vertices.p_unmapped_unnormalized_vert[r_index + 1].vert_data.vert_z = len / 2;
            m_asset.vertices.p_unmapped_unnormalized_vert[r_index + 1].color_data = tmp_col;

            m_asset.vertices.p_unmapped_unnormalized_vert[r_index + 2].vert_data.vert_x = cur_pos + DENG_GRID_THICKNESS;
            m_asset.vertices.p_unmapped_unnormalized_vert[r_index + 2].vert_data.vert_y = 0;
            m_asset.vertices.p_unmapped_unnormalized_vert[r_index + 2].vert_data.vert_z = -(len / 2);
            m_asset.vertices.p_unmapped_unnormalized_vert[r_index + 2].color_data = tmp_col;

            m_asset.indices.p_indices[r_index] = r_index;
            m_asset.indices.p_indices[r_index + 1] = r_index + 1;
            m_asset.indices.p_indices[r_index + 2] = r_index + 2;
            cur_pos += step;
        }
    }

    deng_Asset GridGenerator::getGrid() { return m_asset; }
}
