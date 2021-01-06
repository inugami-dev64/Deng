#define __DENGUI_USAGE 1
#include "../../core/api_core.h"

namespace dengui {
    /* Create new window instance */
    void beginWindow (
        Window **pp_window,
        WindowInfo *p_wi,
        dengMath::vec2<float> draw_bounds,
        Events *p_ev,
        dengUtils::FontManager *p_fm
    ) {
        LOG("seg test");
        (*pp_window) = new Window(p_wi->wt, p_wi->title, p_fm);
        (*pp_window)->setPos(p_wi->pos);
        (*pp_window)->setSize(p_wi->size);
        (*pp_window)->setMB(p_wi->p_mb);
        (*pp_window)->setPC(&p_wi->pc);
        (*pp_window)->setSC(&p_wi->sc);
        (*pp_window)->setTC(&p_wi->tc);
        LOG("seg test");

        std::vector<ElementInfo> elems;
        elems = (*pp_window)->makeWindow(p_wi, draw_bounds);
        LOG("elem_c: " + std::to_string(elems.size()));
        p_ev = new Events(elems, {(uint32_t) draw_bounds.first, (uint32_t) draw_bounds.second});
    }


    /* Make all window elements into DENGAssets */
    void getAssets (
        Window *windows,
        int32_t window_c,
        DENGAsset **p_assets,
        int32_t *p_asset_c,
        dengUtils::bitmapStr **p_bm_strs,
        int32_t *p_bm_str_c
    ) {
        int32_t l_index; 
        size_t r_index, asset_index, bm_index;
        (*p_asset_c) = 0;
        (*p_bm_str_c) = 0;

        std::vector<VERT_UNMAPPED_2D> vert;
        std::vector<uint32_t> indices;
        std::vector<dengUtils::bitmapStr> bm_strs;
        std::vector<UNI_OFFSET> offsets;

        LOG("WINDOW_C: " + std::to_string(window_c));
        // Find the total count off assets
        for(l_index = 0; l_index < window_c; l_index++) {
            offsets = windows[l_index].getOffsets();
            bm_strs = windows[l_index].getTextBoxes();
            (*p_asset_c) += offsets.size();
            (*p_bm_str_c) += bm_strs.size();
        }

        // Allocate memory for assets
        (*p_assets) = (DENGAsset*) calloc (
            (*p_asset_c),
            sizeof(DENGAsset)
        );

        // Allocate memory for bitmap strings
        (*p_bm_strs) = (dengUtils::bitmapStr*) calloc (
            (*p_bm_str_c),
            sizeof(dengUtils::bitmapStr)
        );

        // Create assets from window elements
        asset_index = 0;
        bm_index = 0;
        for(l_index = 0; l_index < window_c; l_index++) {
            vert = windows[l_index].getVerts();
            indices = windows[l_index].getInds();
            offsets = windows[l_index].getOffsets();
            bm_strs = windows[l_index].getTextBoxes();

            for(r_index = 0; r_index < bm_strs.size(); r_index++, bm_index++)
                (*p_bm_strs)[bm_index] = bm_strs[r_index];
                
            // Copy all unmapped vertices data to DENGasset
            for(r_index = 0; r_index < offsets.size(); r_index++, asset_index++) {
                (*p_assets)[asset_index].asset_mode = DENG_ASSET_MODE_2D_UNMAPPED;
                (*p_assets)[asset_index].name = (char*) calloc (
                    strlen(windows[l_index].getTitle().c_str()) + 4,
                    sizeof(char)
                );

                // Allocate memory for asset vertices and indices
                if(r_index != offsets.size() - 1) {
                    (*p_assets)[asset_index].vertices.p_unmapped_vert_data_2d = (VERT_UNMAPPED_2D*) calloc (
                        offsets[r_index + 1].vert_offset - offsets[r_index].vert_offset,
                        sizeof(VERT_UNMAPPED_2D)
                    );
                    (*p_assets)[asset_index].vertices.size = 
                    offsets[r_index + 1].vert_offset - offsets[r_index].vert_offset;

                    (*p_assets)[asset_index].indices.p_indices = (uint32_t*) calloc (
                        offsets[r_index + 1].ind_offset - offsets[r_index].ind_offset,
                        sizeof(uint32_t)
                    );
                    (*p_assets)[asset_index].indices.size = 
                    offsets[r_index + 1].ind_offset - offsets[r_index].ind_offset;
                }

                else {
                    (*p_assets)[asset_index].vertices.p_unmapped_vert_data_2d = (VERT_UNMAPPED_2D*) calloc (
                        vert.size() - offsets[r_index].vert_offset,
                        sizeof(VERT_UNMAPPED_2D)
                    );
                    (*p_assets)[asset_index].vertices.size = 
                    vert.size() - offsets[r_index].vert_offset;

                    (*p_assets)[asset_index].indices.p_indices = (uint32_t*) calloc (
                        indices.size() - offsets[r_index].ind_offset,
                        sizeof(uint32_t)
                    );
                    (*p_assets)[asset_index].indices.size = 
                    indices.size() - offsets[r_index].ind_offset;
                }

                // Write asset description W(DESC)
                sprintf (
                    (*p_assets)[asset_index].name,
                    "W(%s)",
                    windows[l_index].getTitle().c_str()
                );

                // Populate indices and vertices
                if(r_index != offsets.size() - 1) {
                    // Copy vertices
                    memcpy (
                        (*p_assets)[asset_index].vertices.p_unmapped_vert_data_2d,
                        &vert[offsets[r_index].vert_offset],
                        sizeof(VERT_UNMAPPED_2D) * (offsets[r_index + 1].vert_offset - offsets[r_index].vert_offset)
                    );
                    
                    // Copy indices
                    memcpy (
                        (*p_assets)[asset_index].indices.p_indices,
                        &indices[offsets[r_index].ind_offset],
                        sizeof(uint32_t) * (offsets[r_index + 1].ind_offset - offsets[r_index].ind_offset)
                    );
                }

                else {
                    // Copy vertices
                    memcpy (
                        (*p_assets)[asset_index].vertices.p_unmapped_vert_data_2d,
                        &vert[offsets[r_index].vert_offset],
                        sizeof(VERT_UNMAPPED_2D) * (vert.size() - offsets[r_index].vert_offset)
                    );

                    // Copy indices
                    memcpy (
                        (*p_assets)[asset_index].indices.p_indices,
                        &indices[offsets[r_index].ind_offset],
                        sizeof(uint32_t) * (indices.size() - offsets[r_index].ind_offset)
                    );
                }
            }
        }
    }

    
    /* Initialise default values to windowinfo */
    WindowInfo::WindowInfo() {
        p_mb = NULL;
        pc = DEFAULT_PRIMARY_COLOR;
        sc = DEFAULT_SECONDARY_COLOR;
        tc = DEFAULT_TERTIARY_COLOR;
        title = "Hello world!";
        wt = WINDOW_TYPE_FLOATING;
        fl_b = WINDOW_FLAG_NULL;
        pos = DEFAULT_POS;
        size = DEFAULT_SIZE;
    }

    
    /* Window constructor */
    Window::Window (
        WindowType wt, 
        std::string title,
        dengUtils::FontManager *p_fm
    ) {
        m_wt = wt;
        m_title = title;
        m_p_fm = p_fm;
    }


    /* Add rectangle to window class according to absolute postion */
    void Window::addAbsUnmappedRec (
        dengMath::vec2<float> pos, 
        dengMath::vec2<float> size,
        RectangleOrigin rec_origin, 
        OBJColorData color
    ) { 
        // Push offsets to offsets' vector
        UNI_OFFSET offset;
        offset.vert_offset = m_vertices.size();
        offset.ind_offset = m_indices.size();
        m_offsets.push_back(offset);
        
        // Calculate all vertices
        m_vertices.resize(offset.vert_offset + 4);
        m_indices.resize(offset.ind_offset + 6);
        switch (rec_origin)
        {
        case REC_ORIGIN_VERTEX_TOP_LEFT:
            m_vertices[offset.vert_offset].vert_data = *(OBJVerticesData2D*) &pos;
            m_vertices[offset.vert_offset].color_data = color;
            
            m_vertices[offset.vert_offset + 1].vert_data.vert_x = pos.first + size.first;
            m_vertices[offset.vert_offset + 1].vert_data.vert_y = pos.second;
            m_vertices[offset.vert_offset + 1].color_data = color;
            
            m_vertices[offset.vert_offset + 2].vert_data.vert_x = pos.first + size.first;
            m_vertices[offset.vert_offset + 2].vert_data.vert_y = pos.second + size.second;
            m_vertices[offset.vert_offset + 2].color_data = color;
            
            m_vertices[offset.vert_offset + 3].vert_data.vert_x = pos.first;
            m_vertices[offset.vert_offset + 3].vert_data.vert_y = pos.second + size.second;
            m_vertices[offset.vert_offset + 3].color_data = color;
            break;

        case REC_ORIGIN_VERTEX_TOP_RIGHT:
            m_vertices[offset.vert_offset].vert_data.vert_x = pos.first - size.first;
            m_vertices[offset.vert_offset].vert_data.vert_y = pos.second;
            m_vertices[offset.vert_offset].color_data = color;
            
            m_vertices[offset.vert_offset + 1].vert_data = *(OBJVerticesData2D*) &pos;
            m_vertices[offset.vert_offset + 1].color_data = color;
            
            m_vertices[offset.vert_offset + 2].vert_data.vert_x = pos.first;
            m_vertices[offset.vert_offset + 2].vert_data.vert_y = pos.second + size.second;
            m_vertices[offset.vert_offset + 2].color_data = color;
            
            m_vertices[offset.vert_offset + 3].vert_data.vert_x = pos.first - size.first;
            m_vertices[offset.vert_offset + 3].vert_data.vert_y = pos.second + size.second;
            m_vertices[offset.vert_offset + 3].color_data = color;
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_LEFT:
            m_vertices[offset.vert_offset].vert_data.vert_x = pos.first;
            m_vertices[offset.vert_offset].vert_data.vert_y = pos.second - size.second;
            m_vertices[offset.vert_offset].color_data = color;
            
            m_vertices[offset.vert_offset + 1].vert_data.vert_x = pos.first + size.first;
            m_vertices[offset.vert_offset + 1].vert_data.vert_y = pos.second - size.second;
            m_vertices[offset.vert_offset + 1].color_data = color;
            
            m_vertices[offset.vert_offset + 2].vert_data.vert_x = pos.first + size.first;
            m_vertices[offset.vert_offset + 2].vert_data.vert_y = pos.second;
            m_vertices[offset.vert_offset + 2].color_data = color;
            
            m_vertices[offset.vert_offset + 3].vert_data = *(OBJVerticesData2D*) &pos;
            m_vertices[offset.vert_offset + 3].color_data = color;
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_RIGHT:
            m_vertices[offset.vert_offset].vert_data.vert_x = pos.first - size.first;
            m_vertices[offset.vert_offset].vert_data.vert_y = pos.second - size.second;
            m_vertices[offset.vert_offset].color_data = color;
            
            m_vertices[offset.vert_offset + 1].vert_data.vert_x = pos.first;
            m_vertices[offset.vert_offset + 1].vert_data.vert_y = pos.second - size.second;
            m_vertices[offset.vert_offset + 1].color_data = color;
            
            m_vertices[offset.vert_offset + 2].vert_data = *(OBJVerticesData2D*) &pos;
            m_vertices[offset.vert_offset + 2].color_data = color;
            
            m_vertices[offset.vert_offset + 3].vert_data.vert_x = pos.first - size.first;
            m_vertices[offset.vert_offset + 3].vert_data.vert_y = pos.second;
            m_vertices[offset.vert_offset + 3].color_data = color;
            break;
        
        default:
            break;
        }

        // Set all indices
        m_indices[offset.ind_offset] = 0;
        m_indices[offset.ind_offset + 1] = 1;
        m_indices[offset.ind_offset + 2] = 2;
        m_indices[offset.ind_offset + 3] = 2;
        m_indices[offset.ind_offset + 4] = 3;
        m_indices[offset.ind_offset + 5] = 0;
    }


    /* Add rectangle to window class according to relative positions */
    void Window::addRelUnmappedRec (
        dengMath::vec2<float> pos,
        dengMath::vec2<float> size,
        bool is_abs_height,
        RectangleOrigin rec_origin,
        OBJColorData color
    ) {
        OBJVerticesData2D window_top_left = m_vertices[0].vert_data;
        OBJVerticesData2D window_top_right = m_vertices[1].vert_data;
        OBJVerticesData2D window_bottom_left = m_vertices[2].vert_data;
        OBJVerticesData2D window_bottom_right = m_vertices[3].vert_data;

        float window_width = window_top_right.vert_x - window_top_left.vert_x;
        float window_height = window_bottom_left.vert_y - window_top_left.vert_y;

        // Push new offsets to offsets' vector
        UNI_OFFSET offset;
        offset.vert_offset = m_vertices.size();
        offset.ind_offset = m_indices.size();
        m_offsets.push_back(offset);

        // Calculate all vertices
        m_vertices.resize(offset.vert_offset + 4);
        m_indices.resize(offset.ind_offset + 6);
        m_vertices[offset.vert_offset].color_data = color;
        m_vertices[offset.vert_offset + 1].color_data = color;
        m_vertices[offset.vert_offset + 2].color_data = color;
        m_vertices[offset.vert_offset + 3].color_data = color;

        switch (rec_origin)
        {
        case REC_ORIGIN_VERTEX_TOP_LEFT:
            m_vertices[offset.vert_offset].vert_data.vert_x = 
            window_top_left.vert_x + ((pos.first + 1.0f) / 2 * window_width);
            m_vertices[offset.vert_offset + 1].vert_data.vert_x = 
            window_top_left.vert_x + ((pos.first + size.first + 1.0f) / 2 * window_width);
            m_vertices[offset.vert_offset + 2].vert_data.vert_x = 
            window_top_left.vert_x + ((pos.first + size.first + 1.0f) / 2 * window_width);
            m_vertices[offset.vert_offset + 3].vert_data.vert_x = 
            window_top_left.vert_x + ((pos.first + 1.0f) / 2 * window_width);

            if(!is_abs_height) {
                m_vertices[offset.vert_offset].vert_data.vert_y = 
                window_top_left.vert_y + ((pos.second + 1.0f) / 2 * window_height);
                m_vertices[offset.vert_offset + 1].vert_data.vert_y = 
                window_top_left.vert_y + ((pos.second + 1.0f) / 2 * window_height);
                m_vertices[offset.vert_offset + 2].vert_data.vert_y = 
                window_top_left.vert_y + ((pos.second + size.second + 1.0f) / 2 * window_height);
                m_vertices[offset.vert_offset + 3].vert_data.vert_y = 
                window_top_left.vert_y + ((pos.second + size.second + 1.0f) / 2 * window_height);
            }

            else {
                m_vertices[offset.vert_offset].vert_data.vert_y =
                window_top_left.vert_y + (pos.second + 1.0f) / 2 * window_height;
                m_vertices[offset.vert_offset + 1].vert_data.vert_y =
                window_top_left.vert_y + (pos.second + 1.0f) / 2 * window_height;
                m_vertices[offset.vert_offset + 2].vert_data.vert_y =
                window_top_left.vert_y + (pos.second + 1.0f) / 2 * window_height + size.second;
                m_vertices[offset.vert_offset + 3].vert_data.vert_y =
                window_top_left.vert_y + (pos.second + 1.0f) / 2 * window_height + size.second;
            }
            break;

        case REC_ORIGIN_VERTEX_TOP_RIGHT:
            m_vertices[offset.vert_offset].vert_data.vert_x = 
            window_top_right.vert_x - ((pos.first - size.first + 1.0f) / 2 * window_width);
            m_vertices[offset.vert_offset + 1].vert_data.vert_x = 
            window_top_right.vert_x - ((pos.first + 1.0f) / 2 * window_width);
            m_vertices[offset.vert_offset + 2].vert_data.vert_x = 
            window_top_right.vert_x - ((pos.first + 1.0f) / 2 * window_width);
            m_vertices[offset.vert_offset + 3].vert_data.vert_x = 
            window_top_right.vert_x - ((pos.first - size.first + 1.0f) / 2 * window_width);
            
            if(!is_abs_height) {
                m_vertices[offset.vert_offset].vert_data.vert_y = 
                window_top_right.vert_y + ((pos.second + 1.0f) / 2 * window_height);
                m_vertices[offset.vert_offset + 1].vert_data.vert_y = 
                window_top_right.vert_y + ((pos.second + 1.0f) / 2 * window_height);
                m_vertices[offset.vert_offset + 2].vert_data.vert_y = 
                window_top_right.vert_x + ((pos.second + size.second + 1.0f) / 2 * window_height);
                m_vertices[offset.vert_offset + 3].vert_data.vert_y = 
                window_top_right.vert_x + ((pos.second + size.second + 1.0f) / 2 * window_height);
            }

            else {
                m_vertices[offset.vert_offset].vert_data.vert_y =
                window_top_right.vert_y + (pos.second + 1.0f) / 2 * window_height;
                m_vertices[offset.vert_offset + 1].vert_data.vert_y =
                window_top_right.vert_y + (pos.second + 1.0f) / 2 * window_height;
                m_vertices[offset.vert_offset + 2].vert_data.vert_y =
                window_top_right.vert_y + (pos.second + 1.0f) / 2 * window_height + size.second;
                m_vertices[offset.vert_offset + 3].vert_data.vert_y =
                window_top_right.vert_y + (pos.second + 1.0f) / 2 * window_height + size.second;
            }
            break;
        
        case REC_ORIGIN_VERTEX_BOTTOM_RIGHT:
            m_vertices[offset.vert_offset].vert_data.vert_x = 
            window_bottom_left.vert_x - ((pos.first - size.first + 1.0f) / 2 * window_width);
            m_vertices[offset.vert_offset + 1].vert_data.vert_x = 
            window_bottom_left.vert_x - ((pos.first + 1.0f) / 2 * window_width);
            m_vertices[offset.vert_offset + 2].vert_data.vert_x = 
            window_bottom_left.vert_x - ((pos.first + 1.0f) / 2 * window_width);
            m_vertices[offset.vert_offset + 3].vert_data.vert_x = 
            window_bottom_left.vert_x - ((pos.first - size.first + 1.0f) / 2 * window_width);

            if(!is_abs_height) {
                m_vertices[offset.vert_offset].vert_data.vert_y = 
                window_bottom_left.vert_y - ((pos.second - size.second + 1.0f) / 2 * window_height);
                m_vertices[offset.vert_offset + 1].vert_data.vert_y = 
                window_bottom_left.vert_y - ((pos.second - size.second + 1.0f) / 2 * window_height);
                m_vertices[offset.vert_offset + 2].vert_data.vert_y = 
                window_bottom_left.vert_y - ((pos.second + 1.0f) / 2 * window_height);
                m_vertices[offset.vert_offset + 3].vert_data.vert_y = 
                window_bottom_left.vert_y - ((pos.second + 1.0f) / 2 * window_height);
            }

            else {
                m_vertices[offset.vert_offset].vert_data.vert_y =
                window_bottom_left.vert_y - (pos.second + 1.0f) / 2 * window_height - size.second;
                m_vertices[offset.vert_offset + 1].vert_data.vert_y =
                window_bottom_left.vert_y - (pos.second + 1.0f) / 2 * window_height - size.second;
                m_vertices[offset.vert_offset + 2].vert_data.vert_y =
                window_bottom_left.vert_y - (pos.second + 1.0f) / 2 * window_height;
                m_vertices[offset.vert_offset + 3].vert_data.vert_y =
                window_bottom_left.vert_y - (pos.second + 1.0f) / 2 * window_height;
            }
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_LEFT:
            m_vertices[offset.vert_offset].vert_data.vert_x = 
            window_bottom_right.vert_x + ((pos.first + 1.0f) / 2 * window_width);
            m_vertices[offset.vert_offset + 1].vert_data.vert_x = 
            window_bottom_right.vert_x + ((pos.first + size.first + 1.0f) / 2 * window_width);
            m_vertices[offset.vert_offset + 2].vert_data.vert_x = 
            window_bottom_right.vert_x + ((pos.first + size.first + 1.0f) / 2 * window_width);
            m_vertices[offset.vert_offset + 3].vert_data.vert_x = 
            window_bottom_right.vert_x + ((pos.first + 1.0f) / 2 * window_width);
            
            
            if(!is_abs_height) {
                m_vertices[offset.vert_offset].vert_data.vert_y = 
                window_bottom_right.vert_y - ((pos.second - size.second + 1.0f) / 2 * window_height);
                m_vertices[offset.vert_offset + 1].vert_data.vert_y = 
                window_bottom_right.vert_y - ((pos.second - size.second + 1.0f) / 2 * window_height);
                m_vertices[offset.vert_offset + 2].vert_data.vert_y = 
                window_bottom_right.vert_y - ((pos.second + 1.0f) / 2 * window_height);
                m_vertices[offset.vert_offset + 3].vert_data.vert_y = 
                window_bottom_right.vert_y - ((pos.second + 1.0f) / 2 * window_height);
            }

            else {
                m_vertices[offset.vert_offset].vert_data.vert_y =
                window_bottom_right.vert_y - (pos.second + 1.0f) / 2 * window_height - size.second;
                m_vertices[offset.vert_offset + 1].vert_data.vert_y =
                window_bottom_right.vert_y - (pos.second + 1.0f) / 2 * window_height - size.second;
                m_vertices[offset.vert_offset + 2].vert_data.vert_y =
                window_bottom_right.vert_y - (pos.second + 1.0f) / 2 * window_height;
                m_vertices[offset.vert_offset + 3].vert_data.vert_y =
                window_bottom_right.vert_y - (pos.second + 1.0f) / 2 * window_height;
            }
            break;

        default:
            break;
        }

        // Add indices
        m_indices[offset.ind_offset] = 0;
        m_indices[offset.ind_offset + 1] = 1;
        m_indices[offset.ind_offset + 2] = 2;
        m_indices[offset.ind_offset + 3] = 2;
        m_indices[offset.ind_offset + 4] = 3;
        m_indices[offset.ind_offset + 5] = 0;
    }


    /* Add generic triangle element to window */ 
    void Window::addGenTriangle (
        dengMath::vec2<float> pos,
        dengMath::vec2<float> surround_rec_size,
        std::array<dengMath::vec2<float>, 3> rel_tri_coords,
        bool is_abs_size,
        RectangleOrigin rec_origin,
        OBJColorData color
    ) {
        UNI_OFFSET offset;
        offset.ind_offset = m_indices.size();
        offset.vert_offset = m_vertices.size();
        m_offsets.push_back(offset);

        const float window_width = m_vertices[1].vert_data.vert_x - m_vertices[0].vert_data.vert_x;
        const float window_height = m_vertices[3].vert_data.vert_y - m_vertices[0].vert_data.vert_y;
        const OBJVerticesData2D window_top_left = m_vertices[0].vert_data;
        const OBJVerticesData2D window_top_right = m_vertices[1].vert_data;
        const OBJVerticesData2D window_bottom_left = m_vertices[2].vert_data;
        const OBJVerticesData2D window_bottom_right = m_vertices[3].vert_data;
        
        dengMath::vec2<float> surround_rec_coords;
        if(!is_abs_size) {
            surround_rec_size.first = surround_rec_size.first / 2 * window_width;
            surround_rec_size.second = surround_rec_size.second / 2 * window_height;
        }

        m_vertices.resize(offset.vert_offset + 3);
        m_indices.resize(offset.ind_offset + 3);

        // Find the surround rectangle coordinates
        switch (rec_origin)
        {
        case REC_ORIGIN_VERTEX_TOP_LEFT:
            surround_rec_coords.first = 
            window_top_left.vert_x + (pos.first + 1.0f) / 2 * window_width;
            surround_rec_coords.second = 
            window_top_left.vert_y + (pos.second + 1.0f) / 2 * window_height;

            break;

        case REC_ORIGIN_VERTEX_TOP_RIGHT:
            surround_rec_coords.first = 
            window_top_right.vert_x - (pos.first + 1.0f) / 2 * window_width - surround_rec_size.first;
            surround_rec_coords.second = 
            window_top_right.vert_y + (pos.second + 1.0f) / 2 * window_height;
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_LEFT:
            surround_rec_coords.first = 
            window_bottom_left.vert_x + (pos.first + 1.0f) / 2 * window_width;
            surround_rec_coords.second = 
            window_bottom_left.vert_y - (pos.second + 1.0f) / 2 * window_height - surround_rec_size.second;
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_RIGHT:    
            surround_rec_coords.first = 
            window_bottom_right.vert_x - (pos.first + 1.0f) / 2 * window_width - surround_rec_size.first;
            surround_rec_coords.second = 
            window_bottom_right.vert_y - (pos.second + 1.0f) / 2 * window_height - surround_rec_size.second;
            break;
        
        default:
            break;
        }

        m_vertices[offset.vert_offset].vert_data.vert_x = 
        surround_rec_coords.first + (rel_tri_coords[0].first + 1.0f) / 2 * surround_rec_size.first;
        m_vertices[offset.vert_offset].vert_data.vert_y = 
        surround_rec_coords.second + (rel_tri_coords[0].second + 1.0f) / 2 * surround_rec_size.second;
        
        m_vertices[offset.vert_offset + 1].vert_data.vert_x = 
        surround_rec_coords.first + (rel_tri_coords[1].first + 1.0f) / 2 * surround_rec_size.first;
        m_vertices[offset.vert_offset + 1].vert_data.vert_y = 
        surround_rec_coords.second + (rel_tri_coords[1].second + 1.0f) / 2 * surround_rec_size.second;

        m_vertices[offset.vert_offset + 2].vert_data.vert_x = 
        surround_rec_coords.first + (rel_tri_coords[2].first + 1.0f) / 2 * surround_rec_size.first;
        m_vertices[offset.vert_offset + 2].vert_data.vert_y = 
        surround_rec_coords.second + (rel_tri_coords[2].second + 1.0f) / 2 * surround_rec_size.second;

        m_vertices[offset.vert_offset].color_data = color;
        m_vertices[offset.vert_offset + 1].color_data = color;
        m_vertices[offset.vert_offset + 2].color_data = color;
        
        // Add all triangle indices
        m_indices[offset.ind_offset] = 0;
        m_indices[offset.ind_offset + 1] = 1;
        m_indices[offset.ind_offset + 2] = 2;
    }


    /* Add text element to window */
    void Window::addText (
        dengMath::vec2<float> pos,
        float text_size,
        dengMath::vec2<float> draw_bounds,
        RectangleOrigin rec_origin,
        dengUtils::bitmapStr text,
        dengMath::vec3<unsigned char> color
    ) {
        dengError res;
        res = m_p_fm->newVecStr (
            text,
            DEFAULT_FONT_FILE,
            text_size,
            {0.0f, 0.0f},
            color
        );

        if(res != DENG_NO_ERRORS)
            ERR("Failed to create bitmap string for window");

        OBJVerticesData2D window_top_left = m_vertices[0].vert_data;
        OBJVerticesData2D window_top_right = m_vertices[1].vert_data;
        OBJVerticesData2D window_bottom_left = m_vertices[2].vert_data;
        OBJVerticesData2D window_bottom_right = m_vertices[3].vert_data;
        float window_width = window_top_right.vert_x - window_top_left.vert_x;
        float window_height = window_bottom_left.vert_y - window_top_left.vert_y;

        float rec_width = dengMath::Conversion::pixelSizeToVector2DSize (
            (double) text.box_size.first,
            {(uint32_t) draw_bounds.first, (uint32_t) draw_bounds.second},
            DENG_COORD_AXIS_X
        );

        float rec_height = dengMath::Conversion::pixelSizeToVector2DSize (
            (double) text.box_size.second,
            {(uint32_t) draw_bounds.first, (uint32_t) draw_bounds.second},
            DENG_COORD_AXIS_Y
        );

        LOG("TEXT_BOX_SIZE: {" + std::to_string(text.box_size.first) + "," + std::to_string(text.box_size.second));
        LOG("WIDTH, HEIGHT: " + std::to_string(rec_width) + ", " + std::to_string(rec_height));

        switch (rec_origin)
        {
        case REC_ORIGIN_VERTEX_TOP_LEFT:
            text.vert_pos[0].vert_data.vert_x += 
            window_top_left.vert_x + ((pos.first + 1.0f) / 2) * window_width;
            text.vert_pos[0].vert_data.vert_y += 
            window_top_left.vert_y + ((pos.second + 1.0f) / 2) * window_height;

            text.vert_pos[1].vert_data.vert_x += 
            window_top_left.vert_x + ((pos.first + 1.0f) / 2) * window_width;
            text.vert_pos[1].vert_data.vert_y += 
            window_top_left.vert_y + ((pos.second + 1.0f) / 2) * window_height;

            text.vert_pos[2].vert_data.vert_x += 
            window_top_left.vert_x + ((pos.first + 1.0f) / 2) * window_width;
            text.vert_pos[2].vert_data.vert_y += 
            window_top_left.vert_y + ((pos.second + 1.0f) / 2) * window_height;

            text.vert_pos[3].vert_data.vert_x += 
            window_top_left.vert_x + ((pos.first + 1.0f) / 2) * window_width;
            text.vert_pos[3].vert_data.vert_y += 
            window_top_left.vert_y + ((pos.second + 1.0f) / 2) * window_height;
            break;

        case REC_ORIGIN_VERTEX_TOP_RIGHT:
            text.vert_pos[0].vert_data.vert_x += 
            window_top_right.vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[0].vert_data.vert_y += 
            window_top_right.vert_y + ((pos.second + 1.0f) / 2) * window_height;

            text.vert_pos[1].vert_data.vert_x += 
            window_top_right.vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[1].vert_data.vert_y += 
            window_top_right.vert_y + ((pos.second + 1.0f) / 2) * window_height;

            text.vert_pos[2].vert_data.vert_x += 
            window_top_right.vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[2].vert_data.vert_y += 
            window_top_right.vert_y + ((pos.second + 1.0f) / 2) * window_height;

            text.vert_pos[3].vert_data.vert_x += 
            window_top_right.vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[3].vert_data.vert_y += 
            window_top_right.vert_y + ((pos.second + 1.0f) / 2) * window_height;
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_LEFT:
            text.vert_pos[0].vert_data.vert_x += 
            window_bottom_left.vert_x + ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[0].vert_data.vert_y += 
            window_bottom_left.vert_y - ((pos.second + 1.0f) / 2) * window_height + rec_height;

            text.vert_pos[1].vert_data.vert_x += 
            window_bottom_left.vert_x + ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[1].vert_data.vert_y += 
            window_bottom_left.vert_y - ((pos.second + 1.0f) / 2) * window_height + rec_height;

            text.vert_pos[2].vert_data.vert_x += 
            window_bottom_left.vert_x + ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[2].vert_data.vert_y += 
            window_bottom_left.vert_y - ((pos.second + 1.0f) / 2) * window_height + rec_height;

            text.vert_pos[3].vert_data.vert_x += 
            window_bottom_left.vert_x + ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[3].vert_data.vert_y += 
            window_bottom_left.vert_y - ((pos.second + 1.0f) / 2) * window_height + rec_height;
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_RIGHT:
            text.vert_pos[0].vert_data.vert_x += 
            window_bottom_right.vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[0].vert_data.vert_y += 
            window_bottom_right.vert_y - ((pos.second + 1.0f) / 2) * window_height - rec_height;

            text.vert_pos[1].vert_data.vert_x += 
            window_bottom_right.vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[1].vert_data.vert_y += 
            window_bottom_right.vert_y - ((pos.second + 1.0f) / 2) * window_height - rec_height;

            text.vert_pos[2].vert_data.vert_x += 
            window_bottom_right.vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[2].vert_data.vert_y += 
            window_bottom_right.vert_y - ((pos.second + 1.0f) / 2) * window_height - rec_height;

            text.vert_pos[3].vert_data.vert_x += 
            window_bottom_right.vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[3].vert_data.vert_y += 
            window_bottom_right.vert_y - ((pos.second + 1.0f) / 2) * window_height - rec_height;
            break;
        
        default:
            break;
        }

        m_rend_text.push_back(text);
    }


    /* Calculate all window vertices information */
    std::vector<ElementInfo> Window::makeWindow (
        WindowInfo *p_wi,
        dengMath::vec2<float> draw_bounds
    ) {
        // Check for window type and create main window rectangle
        dengMath::vec2<float> tmp_pos;
        dengMath::vec2<float> tmp_size;
        std::array<dengMath::vec2<float>, 3> rel_tri_pos;
        std::vector<ElementInfo> elems;

        switch (p_wi->wt)
        {
        case WINDOW_TYPE_FLOATING:
            // Create floating window rectangle
            addAbsUnmappedRec(
                p_wi->pos, 
                p_wi->size, 
                REC_ORIGIN_VERTEX_TOP_LEFT, 
                {p_wi->pc.first, p_wi->pc.second, p_wi->pc.third, p_wi->pc.fourth}
            );

            rel_tri_pos[0] = {-1.0f, -1.0f};
            rel_tri_pos[1] = {1.0f, -1.0f};
            rel_tri_pos[2] = {0.0f, 1.0f};

            break;

        case WINDOW_TYPE_STATIC_BOTTOM:
            tmp_pos.first = -1.0f;
            tmp_pos.second = 1.0f - p_wi->size.second;
            tmp_size.first = 2.0f;
            tmp_size.second = p_wi->size.second;
            addAbsUnmappedRec (
                tmp_pos, 
                tmp_size, 
                REC_ORIGIN_VERTEX_TOP_LEFT, 
                {p_wi->pc.first, p_wi->pc.second, p_wi->pc.third, p_wi->pc.fourth}
            );

            rel_tri_pos[0] = {-1.0f, -1.0f};
            rel_tri_pos[1] = {1.0f, -1.0f};
            rel_tri_pos[2] = {0.0f, 1.0f};
            break;

        case WINDOW_TYPE_STATIC_TOP:
            tmp_pos.first = -1.0f;
            tmp_pos.second = -1.0f;
            tmp_size.first = 2.0f;
            tmp_size.second = p_wi->size.second;
            addAbsUnmappedRec (
                tmp_pos, 
                tmp_size, 
                REC_ORIGIN_VERTEX_TOP_LEFT, 
                {p_wi->pc.first, p_wi->pc.second, p_wi->pc.third, p_wi->pc.fourth}
            );

            rel_tri_pos[0] = {-1.0f, -1.0f};
            rel_tri_pos[1] = {1.0f, -1.0f};
            rel_tri_pos[2] = {0.0f, 1.0f};
            break;

        case WINDOW_TYPE_STATIC_LEFT:
            tmp_pos.first = -1.0f;
            tmp_pos.second = -1.0f;
            tmp_size.first = p_wi->size.first;
            tmp_size.second = 2.0f;
            addAbsUnmappedRec (
                tmp_pos, 
                tmp_size, 
                REC_ORIGIN_VERTEX_TOP_LEFT, 
                {p_wi->pc.first, p_wi->pc.second, p_wi->pc.third, p_wi->pc.fourth}
            );

            rel_tri_pos[0] = {1.0f, -1.0f};
            rel_tri_pos[1] = {1.0f, 1.0f};
            rel_tri_pos[2] = {-1.0f, 0.0f};
            break;

        case WINDOW_TYPE_STATIC_RIGHT:
            tmp_pos.first = 1.0f - p_wi->size.first;
            tmp_pos.second = -1.0f;
            tmp_size.first = p_wi->size.first;
            tmp_size.second = 2.0f;
            addAbsUnmappedRec (
                tmp_pos, 
                tmp_size, 
                REC_ORIGIN_VERTEX_TOP_LEFT, 
                {p_wi->pc.first, p_wi->pc.second, p_wi->pc.third, p_wi->pc.fourth}
            );

            rel_tri_pos[0] = {-1.0f, -1.0f};
            rel_tri_pos[1] = {-1.0f, 1.0f};
            rel_tri_pos[2] = {1.0f, 0.0f};
            break;
        
        default:
            break;
        }

        // Add titlebar to window
        if(!(p_wi->fl_b & WINDOW_FLAG_NO_TITLEBAR)) {
            tmp_pos.first = -1.0f;
            tmp_pos.second = -1.0f;
            tmp_size.first = 2.0f;
            tmp_size.second = TITLEBAR_HEIGHT;
            addRelUnmappedRec (
                tmp_pos, 
                tmp_size,
                true, 
                REC_ORIGIN_VERTEX_TOP_LEFT,
                *(OBJColorData*) &p_wi->tc
            );

            // Add triangle to minimise if needed
            if(!(p_wi->fl_b & WINDOW_FLAG_NO_COLLAPSE)) {
                tmp_pos.first = -1.0f + (2 * TITLEBAR_ELEM_MARGIN);
                tmp_pos.second = -1.0f + TITLEBAR_ELEM_MARGIN;
                tmp_size.first = TITLEBAR_HEIGHT - (2 * TITLEBAR_ELEM_MARGIN);
                tmp_size.second = TITLEBAR_HEIGHT - (2 * TITLEBAR_ELEM_MARGIN);

                // Add minimizable triangle
                if(p_wi->wt != WINDOW_TYPE_STATIC_LEFT) {
                    addGenTriangle (
                        tmp_pos, 
                        tmp_size,
                        rel_tri_pos,
                        true,
                        REC_ORIGIN_VERTEX_TOP_LEFT,
                        *(OBJColorData*) &p_wi->sc
                    );
                }

                else {
                    addGenTriangle (
                        tmp_pos, 
                        tmp_size,
                        rel_tri_pos,
                        true,
                        REC_ORIGIN_VERTEX_TOP_RIGHT,
                        *(OBJColorData*) &p_wi->sc
                    );
                }

                elems.resize(elems.size() + 1);
                elems[elems.size() - 1].element_name = "Minimize triangle";
                elems[elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_UNMAPPED;
                elems[elems.size() - 1].onLMBClickFunc = minTriangleCallback;

                elems[elems.size() - 1].unmapped_vert.insert ( 
                    elems[elems.size() - 1].unmapped_vert.end(),
                    m_vertices.end() - 3,
                    m_vertices.end()
                );

                elems[elems.size() - 1].indices.insert (
                    elems[elems.size() - 1].indices.end(),
                    m_indices.end() - 3,
                    m_indices.end()
                );
            }

            // // Add close button if needed ([X])
            if(!(p_wi->fl_b & WINDOW_FLAG_NO_CLOSE)) {
                dengUtils::bitmapStr str;
                str.text = "[X]";
                tmp_pos.first = -1.0f + (2 * TITLEBAR_ELEM_MARGIN);
                tmp_pos.second = -1.0f + TITLEBAR_ELEM_MARGIN;
                
                // Add closing button as a text box
                addText (
                    tmp_pos,
                    TITLEBAR_HEIGHT - (2 * TITLEBAR_ELEM_MARGIN),
                    draw_bounds,
                    REC_ORIGIN_VERTEX_TOP_RIGHT,
                    str,
                    {(unsigned char) (p_wi->sc.first * 255), (unsigned char) (p_wi->sc.second * 255), (unsigned char) (p_wi->sc.third * 255)}
                );

                elems.resize(elems.size() + 1);
                elems[elems.size() - 1].element_name = "Close btn";
                elems[elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_TEXTURE_MAPPED;
                elems[elems.size() - 1].onLMBClickFunc = closeBtnCallback;
                
                elems[elems.size() - 1].mapped_vert.insert (
                    elems[elems.size() - 1].mapped_vert.end(),
                    m_rend_text[m_rend_text.size() - 1].vert_pos.begin(),
                    m_rend_text[m_rend_text.size() - 1].vert_pos.end()
                );

                elems[elems.size() - 1].indices.insert (
                    elems[elems.size() - 1].indices.end(),
                    m_rend_text[m_rend_text.size() - 1].vert_indices.begin(),
                    m_rend_text[m_rend_text.size() - 1].vert_indices.end()
                );
            }
        }

        return elems;
    }


    // Window class getters 
    dengMath::vec4<float> Window::getPC() { return m_primary_color; }
    dengMath::vec4<float> Window::getSC() { return m_secondary_color; }
    dengMath::vec4<float> Window::getTC() { return m_tertiary_color; }

    std::vector<VERT_UNMAPPED_2D> Window::getVerts() { return m_vertices; }
    std::vector<uint32_t> Window::getInds() { return m_indices; }
    std::vector<UNI_OFFSET> Window::getOffsets() { return m_offsets; }
    std::string Window::getTitle() { return m_title; }
    std::vector<dengUtils::bitmapStr> Window::getTextBoxes() { return m_rend_text; }


    // Window class setters
    void Window::setPC(dengMath::vec4<float> *p_pc) { m_primary_color = *p_pc; }
    void Window::setSC(dengMath::vec4<float> *p_sc) { m_secondary_color = *p_sc; }
    void Window::setTC(dengMath::vec4<float> *p_tc) { m_tertiary_color = *p_tc; }
    void Window::setPos(dengMath::vec2<float> &pos) { m_pos = pos; }
    void Window::setSize(dengMath::vec2<float> &size) { m_size = size; }
    void Window::setMB(Menubar *p_mb) { m_p_mb = p_mb; };
}