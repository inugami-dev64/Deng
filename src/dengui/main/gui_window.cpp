#define __DENGUI_USAGE 1
#include "../../core/api_core.h"

namespace dengui {
    /* Create new window instance */
    void beginWindow (
        Window **pp_window,
        WindowInfo *p_wi,
        dengMath::vec2<uint32_t> draw_bounds,
        dengUtils::FontManager *p_fm,
        Events *p_ev
    ) {
        (*pp_window) = new Window(p_wi->wt, p_wi->id, p_fm);
        (*pp_window)->setPos(p_wi->pos);
        (*pp_window)->setSize(p_wi->size);
        (*pp_window)->setMB(p_wi->p_mb);
        (*pp_window)->setPC(&p_wi->pc);
        (*pp_window)->setSC(&p_wi->sc);
        (*pp_window)->setTC(&p_wi->tc);

        std::vector<WindowElement> elems;
        (*pp_window)->makeWindow(p_wi, draw_bounds);
        elems = (*pp_window)->getWindowElements();
        p_ev->pushWindowElements(elems);

    }


    /* Start new DENGUI events thread */
    void beginEventHandler (
        Events *p_ev,
        std::vector<deng_Asset> *p_assets,
        std::vector<deng::TextureImageData> *p_textures,
        std::mutex *p_asset_mut,
        deng::DrawCaller *p_dc,
        deng::ResourceAllocator *p_ra,
        deng::DescriptorCreator *p_desc_c,
        VkDevice device,
        VkQueue g_queue,
        VkRenderPass renderpass,
        VkExtent2D extent,
        dengMath::vec4<float> background,
        dengMath::vec2<uint32_t> draw_area
    ) {
        EventInfo ev_info;
        ev_info.background = background;
        ev_info.deng_window_area = draw_area;
        ev_info.device = device;
        ev_info.extent = extent;
        ev_info.g_queue = g_queue;
        ev_info.p_assets = p_assets;
        ev_info.p_dc = p_dc;
        ev_info.p_res_mut = p_asset_mut;
        ev_info.p_textures = p_textures;
        ev_info.renderpass = renderpass;
        p_ev = new Events(ev_info);
    }


    /* Make all window elements into deng_Assets */
    void getAssets (
        Window *windows,
        int32_t window_c,
        deng_Asset **p_assets,
        int32_t *p_asset_c,
        deng_Texture **p_textures,
        int32_t *p_tex_c
    ) {
        int32_t l_index; 
        size_t r_index, asset_index, tex_index;
        std::vector<WindowElement> win_elems;

        (*p_asset_c) = 0;
        (*p_tex_c) = 0;
        // Find the total count off assets
        for(l_index = 0; l_index < window_c; l_index++) {
            win_elems = windows[l_index].getWindowElements();

            for(r_index = 0; r_index < win_elems.size(); r_index++) {
                if(win_elems[r_index].color_mode == ELEMENT_COLOR_MODE_TEXTURE_MAPPED)
                    (*p_tex_c)++;

                (*p_asset_c)++;
            }
        }

        LOG("TEX_C: " + std::to_string((*p_tex_c)));
        // Allocate memory for assets
        if((*p_asset_c)) {
            (*p_assets) = (deng_Asset*) calloc (
                (*p_asset_c),
                sizeof(deng_Asset)
            );
        }

        // Allocate memory for textures
        if((*p_tex_c)) {
            (*p_textures) = (deng_Texture*) calloc (
                (*p_tex_c),
                sizeof(deng_Texture)
            );
        }

        // Create assets from window elements
        asset_index = 0;
        tex_index = 0;
        for(l_index = 0; l_index < window_c; l_index++) {
            win_elems = windows[l_index].getWindowElements();
            
            // Iterate through elements
            for(r_index = 0; r_index < win_elems.size(); r_index++) {
                // Create ID string
                win_elems[r_index].asset_id = (char*) calloc (
                    win_elems[r_index].child_id.size() + win_elems[r_index].parent_id.size() + 5,
                    sizeof(char)
                );

                sprintf (
                    win_elems[r_index].asset_id,
                    "W(%s/%s)",
                    win_elems[r_index].child_id.c_str(),
                    win_elems[r_index].parent_id.c_str()
                );

                switch (win_elems[r_index].color_mode)
                {
                case ELEMENT_COLOR_MODE_TEXTURE_MAPPED:
                    // Populate texture information
                    (*p_textures)[tex_index].pixel_data.width = win_elems[r_index].tex_box.first;
                    (*p_textures)[tex_index].pixel_data.height = win_elems[r_index].tex_box.second;
                    (*p_textures)[tex_index].pixel_data.size = 
                    win_elems[r_index].tex_box.first * win_elems[r_index].tex_box.second * 4;
                    (*p_textures)[tex_index].id = win_elems[r_index].asset_id;
                    
                    // Allocate memory for texture data and copy all pixels over
                    LOG("tex_size: " + std::to_string(win_elems[r_index].texture.size()));
                    (*p_textures)[tex_index].pixel_data.p_pixel_data = (uint8_t*) calloc (
                        win_elems[r_index].texture.size(),
                        sizeof(uint8_t)
                    );
                    
                    memcpy (
                        (*p_textures)[tex_index].pixel_data.p_pixel_data,
                        win_elems[r_index].texture.data(),
                        win_elems[r_index].texture.size() * sizeof(uint8_t)
                    );


                    // Populate texture mapped asset vertices
                    (*p_assets)[asset_index].id = win_elems[r_index].asset_id;
                    (*p_assets)[asset_index].name = win_elems[r_index].asset_id;
                    (*p_assets)[asset_index].asset_mode = DENG_ASSET_MODE_2D_TEXTURE_MAPPED;
                    (*p_assets)[asset_index].is_shown = win_elems[r_index].is_visible;
                    (*p_assets)[asset_index].vertices.size = win_elems[r_index].mapped_vert.size();
                    (*p_assets)[asset_index].vertices.p_texture_mapped_vert_data_2d = (VERT_MAPPED_2D*) calloc (
                        (*p_assets)[asset_index].vertices.size,
                        sizeof(VERT_MAPPED_2D)
                    );

                    memcpy (
                        (*p_assets)[asset_index].vertices.p_texture_mapped_vert_data_2d,
                        win_elems[r_index].mapped_vert.data(),
                        sizeof(VERT_MAPPED_2D) * (*p_assets)[asset_index].vertices.size
                    );

                    tex_index++;
                    break;

                case ELEMENT_COLOR_MODE_UNMAPPED:
                    // Populate unmapped asset vertices
                    (*p_assets)[asset_index].id = win_elems[r_index].asset_id;
                    (*p_assets)[asset_index].name = (*p_assets)[asset_index].id;
                    (*p_assets)[asset_index].asset_mode = DENG_ASSET_MODE_2D_UNMAPPED;
                    (*p_assets)[asset_index].is_shown = win_elems[r_index].is_visible;
                    (*p_assets)[asset_index].vertices.size = win_elems[r_index].unmapped_vert.size();
                    (*p_assets)[asset_index].vertices.p_unmapped_vert_data_2d = (VERT_UNMAPPED_2D*) calloc (
                        (*p_assets)[asset_index].vertices.size,
                        sizeof(VERT_UNMAPPED_2D)
                    );

                    memcpy (
                        (*p_assets)[asset_index].vertices.p_unmapped_vert_data_2d,
                        win_elems[r_index].unmapped_vert.data(),
                        sizeof(VERT_UNMAPPED_2D) * (*p_assets)[asset_index].vertices.size
                    );
                    break;
                
                default:
                    break;
                }

                // Populate asset indices
                (*p_assets)[asset_index].indices.size = win_elems[r_index].indices.size();
                (*p_assets)[asset_index].indices.p_indices = (uint32_t*) calloc (
                    (*p_assets)[asset_index].indices.size,
                    sizeof(uint32_t)
                );

                memcpy (
                    (*p_assets)[asset_index].indices.p_indices,
                    win_elems[r_index].indices.data(),
                    sizeof(uint32_t) * (*p_assets)[asset_index].indices.size
                );

                asset_index++;
            }
        }
    }

    
    /* Initialise default values to windowinfo */
    WindowInfo::WindowInfo() {
        p_mb = NULL;
        pc = DENGUI_DEFAULT_PRIMARY_COLOR;
        sc = DENGUI_DEFAULT_SECONDARY_COLOR;
        tc = DENGUI_DEFAULT_TERTIARY_COLOR;
        id = "Hello world!";
        wt = WINDOW_TYPE_FLOATING;
        fl_b = DENGUI_WINDOW_FLAG_NULL;
        pos = DENGUI_DEFAULT_POS;
        size = DENGUI_DEFAULT_SIZE;
    }

    
    /* Window constructor */
    Window::Window (
        WindowType wt, 
        std::string id,
        dengUtils::FontManager *p_fm
    ) {
        m_wt = wt;
        m_id = id;
        m_p_fm = p_fm;
    }


    /* Add rectangle to window class according to absolute postion */
    void Window::addAbsUnmappedRec (
        dengMath::vec2<float> pos, 
        dengMath::vec2<float> size,
        RectangleOrigin rec_origin, 
        deng_ObjColorData color,
        std::vector<VERT_UNMAPPED_2D> &vert,
        std::vector<uint32_t> &indices
    ) { 
        // Calculate all vertices
        UNI_OFFSET offset;
        offset.vert_offset = vert.size();
        offset.ind_offset = indices.size();

        vert.resize(offset.vert_offset + 4);
        indices.resize(offset.ind_offset + 6);
        switch (rec_origin)
        {
        case REC_ORIGIN_VERTEX_TOP_LEFT:
            vert[offset.vert_offset].vert_data = *(deng_ObjVertData2D*) &pos;
            vert[offset.vert_offset].color_data = color;
            
            vert[offset.vert_offset + 1].vert_data.vert_x = pos.first + size.first;
            vert[offset.vert_offset + 1].vert_data.vert_y = pos.second;
            vert[offset.vert_offset + 1].color_data = color;
            
            vert[offset.vert_offset + 2].vert_data.vert_x = pos.first + size.first;
            vert[offset.vert_offset + 2].vert_data.vert_y = pos.second + size.second;
            vert[offset.vert_offset + 2].color_data = color;
            
            vert[offset.vert_offset + 3].vert_data.vert_x = pos.first;
            vert[offset.vert_offset + 3].vert_data.vert_y = pos.second + size.second;
            vert[offset.vert_offset + 3].color_data = color;
            break;

        case REC_ORIGIN_VERTEX_TOP_RIGHT:
            vert[offset.vert_offset].vert_data.vert_x = pos.first - size.first;
            vert[offset.vert_offset].vert_data.vert_y = pos.second;
            vert[offset.vert_offset].color_data = color;
            
            vert[offset.vert_offset + 1].vert_data = *(deng_ObjVertData2D*) &pos;
            vert[offset.vert_offset + 1].color_data = color;
            
            vert[offset.vert_offset + 2].vert_data.vert_x = pos.first;
            vert[offset.vert_offset + 2].vert_data.vert_y = pos.second + size.second;
            vert[offset.vert_offset + 2].color_data = color;
            
            vert[offset.vert_offset + 3].vert_data.vert_x = pos.first - size.first;
            vert[offset.vert_offset + 3].vert_data.vert_y = pos.second + size.second;
            vert[offset.vert_offset + 3].color_data = color;
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_LEFT:
            vert[offset.vert_offset].vert_data.vert_x = pos.first;
            vert[offset.vert_offset].vert_data.vert_y = pos.second - size.second;
            vert[offset.vert_offset].color_data = color;
            
            vert[offset.vert_offset + 1].vert_data.vert_x = pos.first + size.first;
            vert[offset.vert_offset + 1].vert_data.vert_y = pos.second - size.second;
            vert[offset.vert_offset + 1].color_data = color;
            
            vert[offset.vert_offset + 2].vert_data.vert_x = pos.first + size.first;
            vert[offset.vert_offset + 2].vert_data.vert_y = pos.second;
            vert[offset.vert_offset + 2].color_data = color;
            
            vert[offset.vert_offset + 3].vert_data = *(deng_ObjVertData2D*) &pos;
            vert[offset.vert_offset + 3].color_data = color;
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_RIGHT:
            vert[offset.vert_offset].vert_data.vert_x = pos.first - size.first;
            vert[offset.vert_offset].vert_data.vert_y = pos.second - size.second;
            vert[offset.vert_offset].color_data = color;
            
            vert[offset.vert_offset + 1].vert_data.vert_x = pos.first;
            vert[offset.vert_offset + 1].vert_data.vert_y = pos.second - size.second;
            vert[offset.vert_offset + 1].color_data = color;
            
            vert[offset.vert_offset + 2].vert_data = *(deng_ObjVertData2D*) &pos;
            vert[offset.vert_offset + 2].color_data = color;
            
            vert[offset.vert_offset + 3].vert_data.vert_x = pos.first - size.first;
            vert[offset.vert_offset + 3].vert_data.vert_y = pos.second;
            vert[offset.vert_offset + 3].color_data = color;
            break;
        
        default:
            break;
        }

        // Set all indices
        indices[offset.ind_offset] = 0;
        indices[offset.ind_offset + 1] = 1;
        indices[offset.ind_offset + 2] = 2;
        indices[offset.ind_offset + 3] = 2;
        indices[offset.ind_offset + 4] = 3;
        indices[offset.ind_offset + 5] = 0;
    }


    /* Add rectangle to window class according to relative positions */
    void Window::addRelUnmappedRec (
        dengMath::vec2<float> pos,
        dengMath::vec2<float> size,
        bool is_abs_height,
        RectangleOrigin rec_origin,
        deng_ObjColorData color,
        std::array<deng_ObjVertData2D, 4> &form_vert,
        std::vector<VERT_UNMAPPED_2D> &vert,
        std::vector<uint32_t> &indices
    ) {
        float window_width = form_vert[1].vert_x - form_vert[0].vert_x;
        float window_height = form_vert[3].vert_y - form_vert[0].vert_y;

        // Push new offsets to offsets' vector
        UNI_OFFSET offset;
        offset.vert_offset = vert.size();
        offset.ind_offset = indices.size();

        // Calculate all vertices
        vert.resize(offset.vert_offset + 4);
        indices.resize(offset.ind_offset + 6);
        vert[offset.vert_offset].color_data = color;
        vert[offset.vert_offset + 1].color_data = color;
        vert[offset.vert_offset + 2].color_data = color;
        vert[offset.vert_offset + 3].color_data = color;

        switch (rec_origin)
        {
        case REC_ORIGIN_VERTEX_TOP_LEFT:
            vert[offset.vert_offset].vert_data.vert_x = 
            form_vert[0].vert_x + ((pos.first + 1.0f) / 2 * window_width);
            vert[offset.vert_offset + 1].vert_data.vert_x = 
            form_vert[0].vert_x + ((pos.first + size.first + 1.0f) / 2 * window_width);
            vert[offset.vert_offset + 2].vert_data.vert_x = 
            form_vert[0].vert_x + ((pos.first + size.first + 1.0f) / 2 * window_width);
            vert[offset.vert_offset + 3].vert_data.vert_x = 
            form_vert[0].vert_x + ((pos.first + 1.0f) / 2 * window_width);

            if(!is_abs_height) {
                vert[offset.vert_offset].vert_data.vert_y = 
                form_vert[0].vert_y + ((pos.second + 1.0f) / 2 * window_height);
                vert[offset.vert_offset + 1].vert_data.vert_y = 
                form_vert[0].vert_y + ((pos.second + 1.0f) / 2 * window_height);
                vert[offset.vert_offset + 2].vert_data.vert_y = 
                form_vert[0].vert_y + ((pos.second + size.second + 1.0f) / 2 * window_height);
                vert[offset.vert_offset + 3].vert_data.vert_y = 
                form_vert[0].vert_y + ((pos.second + size.second + 1.0f) / 2 * window_height);
            }

            else {
                vert[offset.vert_offset].vert_data.vert_y =
                form_vert[0].vert_y + (pos.second + 1.0f) / 2 * window_height;
                vert[offset.vert_offset + 1].vert_data.vert_y =
                form_vert[0].vert_y + (pos.second + 1.0f) / 2 * window_height;
                vert[offset.vert_offset + 2].vert_data.vert_y =
                form_vert[0].vert_y + (pos.second + 1.0f) / 2 * window_height + size.second;
                vert[offset.vert_offset + 3].vert_data.vert_y =
                form_vert[0].vert_y + (pos.second + 1.0f) / 2 * window_height + size.second;
            }
            break;

        case REC_ORIGIN_VERTEX_TOP_RIGHT:
            vert[offset.vert_offset].vert_data.vert_x = 
            form_vert[1].vert_x - ((pos.first - size.first + 1.0f) / 2 * window_width);
            vert[offset.vert_offset + 1].vert_data.vert_x = 
            form_vert[1].vert_x - ((pos.first + 1.0f) / 2 * window_width);
            vert[offset.vert_offset + 2].vert_data.vert_x = 
            form_vert[1].vert_x - ((pos.first + 1.0f) / 2 * window_width);
            vert[offset.vert_offset + 3].vert_data.vert_x = 
            form_vert[1].vert_x - ((pos.first - size.first + 1.0f) / 2 * window_width);
            
            if(!is_abs_height) {
                vert[offset.vert_offset].vert_data.vert_y = 
                form_vert[1].vert_y + ((pos.second + 1.0f) / 2 * window_height);
                vert[offset.vert_offset + 1].vert_data.vert_y = 
                form_vert[1].vert_y + ((pos.second + 1.0f) / 2 * window_height);
                vert[offset.vert_offset + 2].vert_data.vert_y = 
                form_vert[1].vert_x + ((pos.second + size.second + 1.0f) / 2 * window_height);
                vert[offset.vert_offset + 3].vert_data.vert_y = 
                form_vert[1].vert_x + ((pos.second + size.second + 1.0f) / 2 * window_height);
            }

            else {
                vert[offset.vert_offset].vert_data.vert_y =
                form_vert[1].vert_y + (pos.second + 1.0f) / 2 * window_height;
                vert[offset.vert_offset + 1].vert_data.vert_y =
                form_vert[1].vert_y + (pos.second + 1.0f) / 2 * window_height;
                vert[offset.vert_offset + 2].vert_data.vert_y =
                form_vert[1].vert_y + (pos.second + 1.0f) / 2 * window_height + size.second;
                vert[offset.vert_offset + 3].vert_data.vert_y =
                form_vert[1].vert_y + (pos.second + 1.0f) / 2 * window_height + size.second;
            }
            break;
        
        case REC_ORIGIN_VERTEX_BOTTOM_RIGHT:
            vert[offset.vert_offset].vert_data.vert_x = 
            form_vert[3].vert_x - ((pos.first - size.first + 1.0f) / 2 * window_width);
            vert[offset.vert_offset + 1].vert_data.vert_x = 
            form_vert[3].vert_x - ((pos.first + 1.0f) / 2 * window_width);
            vert[offset.vert_offset + 2].vert_data.vert_x = 
            form_vert[3].vert_x - ((pos.first + 1.0f) / 2 * window_width);
            vert[offset.vert_offset + 3].vert_data.vert_x = 
            form_vert[3].vert_x - ((pos.first - size.first + 1.0f) / 2 * window_width);

            if(!is_abs_height) {
                vert[offset.vert_offset].vert_data.vert_y = 
                form_vert[3].vert_y - ((pos.second - size.second + 1.0f) / 2 * window_height);
                vert[offset.vert_offset + 1].vert_data.vert_y = 
                form_vert[3].vert_y - ((pos.second - size.second + 1.0f) / 2 * window_height);
                vert[offset.vert_offset + 2].vert_data.vert_y = 
                form_vert[3].vert_y - ((pos.second + 1.0f) / 2 * window_height);
                vert[offset.vert_offset + 3].vert_data.vert_y = 
                form_vert[3].vert_y - ((pos.second + 1.0f) / 2 * window_height);
            }

            else {
                vert[offset.vert_offset].vert_data.vert_y =
                form_vert[3].vert_y - (pos.second + 1.0f) / 2 * window_height - size.second;
                vert[offset.vert_offset + 1].vert_data.vert_y =
                form_vert[3].vert_y - (pos.second + 1.0f) / 2 * window_height - size.second;
                vert[offset.vert_offset + 2].vert_data.vert_y =
                form_vert[3].vert_y - (pos.second + 1.0f) / 2 * window_height;
                vert[offset.vert_offset + 3].vert_data.vert_y =
                form_vert[3].vert_y - (pos.second + 1.0f) / 2 * window_height;
            }
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_LEFT:
            vert[offset.vert_offset].vert_data.vert_x = 
            form_vert[2].vert_x + ((pos.first + 1.0f) / 2 * window_width);
            vert[offset.vert_offset + 1].vert_data.vert_x = 
            form_vert[2].vert_x + ((pos.first + size.first + 1.0f) / 2 * window_width);
            vert[offset.vert_offset + 2].vert_data.vert_x = 
            form_vert[2].vert_x + ((pos.first + size.first + 1.0f) / 2 * window_width);
            vert[offset.vert_offset + 3].vert_data.vert_x = 
            form_vert[2].vert_x + ((pos.first + 1.0f) / 2 * window_width);
            
            
            if(!is_abs_height) {
                vert[offset.vert_offset].vert_data.vert_y = 
                form_vert[2].vert_y - ((pos.second - size.second + 1.0f) / 2 * window_height);
                vert[offset.vert_offset + 1].vert_data.vert_y = 
                form_vert[2].vert_y - ((pos.second - size.second + 1.0f) / 2 * window_height);
                vert[offset.vert_offset + 2].vert_data.vert_y = 
                form_vert[2].vert_y - ((pos.second + 1.0f) / 2 * window_height);
                vert[offset.vert_offset + 3].vert_data.vert_y = 
                form_vert[2].vert_y - ((pos.second + 1.0f) / 2 * window_height);
            }

            else {
                vert[offset.vert_offset].vert_data.vert_y =
                form_vert[2].vert_y - (pos.second + 1.0f) / 2 * window_height - size.second;
                vert[offset.vert_offset + 1].vert_data.vert_y =
                form_vert[2].vert_y - (pos.second + 1.0f) / 2 * window_height - size.second;
                vert[offset.vert_offset + 2].vert_data.vert_y =
                form_vert[2].vert_y - (pos.second + 1.0f) / 2 * window_height;
                vert[offset.vert_offset + 3].vert_data.vert_y =
                form_vert[2].vert_y - (pos.second + 1.0f) / 2 * window_height;
            }
            break;

        default:
            break;
        }

        // Add indices
        indices[offset.ind_offset] = 0;
        indices[offset.ind_offset + 1] = 1;
        indices[offset.ind_offset + 2] = 2;
        indices[offset.ind_offset + 3] = 2;
        indices[offset.ind_offset + 4] = 3;
        indices[offset.ind_offset + 5] = 0;
    }


    /* Add generic triangle element to window */ 
    void Window::addGenTriangle (
        dengMath::vec2<float> pos,
        dengMath::vec2<float> surround_rec_size,
        std::array<dengMath::vec2<float>, 3> rel_tri_coords,
        bool is_abs_size,
        RectangleOrigin rec_origin,
        deng_ObjColorData color,
        std::array<deng_ObjVertData2D, 4> &form_vert,
        std::vector<VERT_UNMAPPED_2D> &vert,
        std::vector<uint32_t> &indices
    ) {
        UNI_OFFSET offset;
        offset.ind_offset = indices.size();
        offset.vert_offset = vert.size();

        const float window_width = form_vert[1].vert_x - form_vert[0].vert_x;
        const float window_height = form_vert[3].vert_y - form_vert[0].vert_y;
        
        dengMath::vec2<float> surround_rec_coords;
        if(!is_abs_size) {
            surround_rec_size.first = surround_rec_size.first / 2 * window_width;
            surround_rec_size.second = surround_rec_size.second / 2 * window_height;
        }

        vert.resize(offset.vert_offset + 3);
        indices.resize(offset.ind_offset + 3);

        // Find the surround rectangle coordinates
        switch (rec_origin)
        {
        case REC_ORIGIN_VERTEX_TOP_LEFT:
            surround_rec_coords.first = 
            form_vert[0].vert_x + (pos.first + 1.0f) / 2 * window_width;
            surround_rec_coords.second = 
            form_vert[0].vert_y + (pos.second + 1.0f) / 2 * window_height;

            break;

        case REC_ORIGIN_VERTEX_TOP_RIGHT:
            surround_rec_coords.first = 
            form_vert[1].vert_x - (pos.first + 1.0f) / 2 * window_width - surround_rec_size.first;
            surround_rec_coords.second = 
            form_vert[1].vert_y + (pos.second + 1.0f) / 2 * window_height;
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_LEFT:
            surround_rec_coords.first = 
            form_vert[3].vert_x + (pos.first + 1.0f) / 2 * window_width;
            surround_rec_coords.second = 
            form_vert[3].vert_y - (pos.second + 1.0f) / 2 * window_height - surround_rec_size.second;
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_RIGHT:    
            surround_rec_coords.first = 
            form_vert[2].vert_x - (pos.first + 1.0f) / 2 * window_width - surround_rec_size.first;
            surround_rec_coords.second = 
            form_vert[2].vert_y - (pos.second + 1.0f) / 2 * window_height - surround_rec_size.second;
            break;
        
        default:
            break;
        }

        vert[offset.vert_offset].vert_data.vert_x = 
        surround_rec_coords.first + (rel_tri_coords[0].first + 1.0f) / 2 * surround_rec_size.first;
        vert[offset.vert_offset].vert_data.vert_y = 
        surround_rec_coords.second + (rel_tri_coords[0].second + 1.0f) / 2 * surround_rec_size.second;
        
        vert[offset.vert_offset + 1].vert_data.vert_x = 
        surround_rec_coords.first + (rel_tri_coords[1].first + 1.0f) / 2 * surround_rec_size.first;
        vert[offset.vert_offset + 1].vert_data.vert_y = 
        surround_rec_coords.second + (rel_tri_coords[1].second + 1.0f) / 2 * surround_rec_size.second;

        vert[offset.vert_offset + 2].vert_data.vert_x = 
        surround_rec_coords.first + (rel_tri_coords[2].first + 1.0f) / 2 * surround_rec_size.first;
        vert[offset.vert_offset + 2].vert_data.vert_y = 
        surround_rec_coords.second + (rel_tri_coords[2].second + 1.0f) / 2 * surround_rec_size.second;

        vert[offset.vert_offset].color_data = color;
        vert[offset.vert_offset + 1].color_data = color;
        vert[offset.vert_offset + 2].color_data = color;
        
        // Add all triangle indices
        indices[offset.ind_offset] = 0;
        indices[offset.ind_offset + 1] = 1;
        indices[offset.ind_offset + 2] = 2;
    }


    /* Add text element to window */
    void Window::addText (
        dengMath::vec2<float> pos,
        float text_size,
        dengMath::vec2<uint32_t> draw_bounds,
        RectangleOrigin rec_origin,
        dengUtils::bitmapStr text,
        dengMath::vec3<unsigned char> color,
        std::array<deng_ObjVertData2D, 4> &form_vert,
        std::vector<VERT_MAPPED_2D> &vert,
        std::vector<uint32_t> &indices,
        std::vector<uint8_t> &tex,
        dengMath::vec2<int32_t> &tex_size
    ) {
        deng_Error res;
        res = m_p_fm->newVecStr (
            text,
            DENGUI_DEFAULT_FONT_FILE,
            text_size,
            {0.0f, 0.0f},
            color
        );

        if(res != DENG_NO_ERRORS)
            FONT_ERR("failed to create text bitmap for DENGUI window");

        float window_width = form_vert[1].vert_x - form_vert[0].vert_x;
        float window_height = form_vert[3].vert_y - form_vert[0].vert_y;

        float rec_width = dengMath::Conversion::pixelSizeToVector2DSize (
            (double) text.box_size.first,
            {draw_bounds.first, (uint32_t) draw_bounds.second},
            DENG_COORD_AXIS_X
        );

        float rec_height = dengMath::Conversion::pixelSizeToVector2DSize (
            (double) text.box_size.second,
            {draw_bounds.first, (uint32_t) draw_bounds.second},
            DENG_COORD_AXIS_Y
        );

        switch (rec_origin)
        {
        case REC_ORIGIN_VERTEX_TOP_LEFT:
            text.vert_pos[0].vert_data.vert_x += 
            form_vert[0].vert_x + ((pos.first + 1.0f) / 2) * window_width;
            text.vert_pos[0].vert_data.vert_y += 
            form_vert[0].vert_y + ((pos.second + 1.0f) / 2) * window_height;

            text.vert_pos[1].vert_data.vert_x += 
            form_vert[0].vert_x + ((pos.first + 1.0f) / 2) * window_width;
            text.vert_pos[1].vert_data.vert_y += 
            form_vert[0].vert_y + ((pos.second + 1.0f) / 2) * window_height;

            text.vert_pos[2].vert_data.vert_x += 
            form_vert[0].vert_x + ((pos.first + 1.0f) / 2) * window_width;
            text.vert_pos[2].vert_data.vert_y += 
            form_vert[0].vert_y + ((pos.second + 1.0f) / 2) * window_height;


            text.vert_pos[3].vert_data.vert_x += 
            form_vert[0].vert_x + ((pos.first + 1.0f) / 2) * window_width;
            text.vert_pos[3].vert_data.vert_y += 
            form_vert[0].vert_y + ((pos.second + 1.0f) / 2) * window_height;
            break;

        case REC_ORIGIN_VERTEX_TOP_RIGHT:
            text.vert_pos[0].vert_data.vert_x += 
            form_vert[1].vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[0].vert_data.vert_y += 
            form_vert[1].vert_y + ((pos.second + 1.0f) / 2) * window_height;

            text.vert_pos[1].vert_data.vert_x += 
            form_vert[1].vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[1].vert_data.vert_y += 
            form_vert[1].vert_y + ((pos.second + 1.0f) / 2) * window_height;

            text.vert_pos[2].vert_data.vert_x += 
            form_vert[1].vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[2].vert_data.vert_y += 
            form_vert[1].vert_y + ((pos.second + 1.0f) / 2) * window_height;

            text.vert_pos[3].vert_data.vert_x += 
            form_vert[1].vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[3].vert_data.vert_y += 
            form_vert[1].vert_y + ((pos.second + 1.0f) / 2) * window_height;
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_LEFT:
            text.vert_pos[0].vert_data.vert_x += 
            form_vert[3].vert_x + ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[0].vert_data.vert_y += 
            form_vert[3].vert_y - ((pos.second + 1.0f) / 2) * window_height + rec_height;

            text.vert_pos[1].vert_data.vert_x += 
            form_vert[3].vert_x + ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[1].vert_data.vert_y += 
            form_vert[3].vert_y - ((pos.second + 1.0f) / 2) * window_height + rec_height;

            text.vert_pos[2].vert_data.vert_x += 
            form_vert[3].vert_x + ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[2].vert_data.vert_y += 
            form_vert[3].vert_y - ((pos.second + 1.0f) / 2) * window_height + rec_height;

            text.vert_pos[3].vert_data.vert_x += 
            form_vert[3].vert_x + ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[3].vert_data.vert_y += 
            form_vert[3].vert_y - ((pos.second + 1.0f) / 2) * window_height + rec_height;
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_RIGHT:
            text.vert_pos[0].vert_data.vert_x += 
            form_vert[2].vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[0].vert_data.vert_y += 
            form_vert[2].vert_y - ((pos.second + 1.0f) / 2) * window_height - rec_height;

            text.vert_pos[1].vert_data.vert_x += 
            form_vert[2].vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[1].vert_data.vert_y += 
            form_vert[2].vert_y - ((pos.second + 1.0f) / 2) * window_height - rec_height;

            text.vert_pos[2].vert_data.vert_x += 
            form_vert[2].vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[2].vert_data.vert_y += 
            form_vert[2].vert_y - ((pos.second + 1.0f) / 2) * window_height - rec_height;

            text.vert_pos[3].vert_data.vert_x += 
            form_vert[2].vert_x - ((pos.first + 1.0f) / 2) * window_width - rec_width;
            text.vert_pos[3].vert_data.vert_y += 
            form_vert[2].vert_y - ((pos.second + 1.0f) / 2) * window_height - rec_height;
            break;
        
        default:
            break;
        }
        
        vert.insert (
            vert.end(),
            text.vert_pos.begin(),
            text.vert_pos.end()
        );

        indices.insert (
            indices.end(),
            text.vert_indices.begin(),
            text.vert_indices.end()
        );

        tex.insert (
            tex.end(),
            text.tex_data.begin(),
            text.tex_data.end()
        );

        tex_size = text.box_size;
    }


    /* Calculate all window vertices information */
    void Window::makeWindow (
        WindowInfo *p_wi,
        dengMath::vec2<uint32_t> draw_bounds
    ) {
        // Check for window type and create main window rectangle
        dengMath::vec2<float> tmp_pos;
        dengMath::vec2<float> tmp_size;
        std::array<dengMath::vec2<float>, 3> rel_tri_pos;
        m_win_elems.resize(m_win_elems.size() + 1);

        switch (p_wi->wt)
        {
        case WINDOW_TYPE_FLOATING:
            // Create floating window rectangle
            addAbsUnmappedRec(
                p_wi->pos, 
                p_wi->size, 
                REC_ORIGIN_VERTEX_TOP_LEFT, 
                {p_wi->pc.first, p_wi->pc.second, p_wi->pc.third, p_wi->pc.fourth},
                m_win_elems[m_win_elems.size() - 1].unmapped_vert,
                m_win_elems[m_win_elems.size() - 1].indices
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
                {p_wi->pc.first, p_wi->pc.second, p_wi->pc.third, p_wi->pc.fourth},
                m_win_elems[m_win_elems.size() - 1].unmapped_vert,
                m_win_elems[m_win_elems.size() - 1].indices
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
                {p_wi->pc.first, p_wi->pc.second, p_wi->pc.third, p_wi->pc.fourth},
                m_win_elems[m_win_elems.size() - 1].unmapped_vert,
                m_win_elems[m_win_elems.size() - 1].indices
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
                {p_wi->pc.first, p_wi->pc.second, p_wi->pc.third, p_wi->pc.fourth},
                m_win_elems[m_win_elems.size() - 1].unmapped_vert,
                m_win_elems[m_win_elems.size() - 1].indices
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
                {p_wi->pc.first, p_wi->pc.second, p_wi->pc.third, p_wi->pc.fourth},
                m_win_elems[m_win_elems.size() - 1].unmapped_vert,
                m_win_elems[m_win_elems.size() - 1].indices
            );

            rel_tri_pos[0] = {-1.0f, -1.0f};
            rel_tri_pos[1] = {-1.0f, 1.0f};
            rel_tri_pos[2] = {1.0f, 0.0f};
            break;
        
        default:
            break;
        }

        m_win_elems[m_win_elems.size() - 1].parent_id = p_wi->id;
        m_win_elems[m_win_elems.size() - 1].child_id = DENGUI_FORM_ID;
        m_win_elems[m_win_elems.size() - 1].is_visible = true;
        m_win_elems[m_win_elems.size() - 1].is_interactive = false;
        m_win_elems[m_win_elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_UNMAPPED;
        m_win_elems[m_win_elems.size() - 1].onLMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onMMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onRMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrUpFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrDownFunc = NULL;
        std::array<deng_ObjVertData2D, 4> form_vert;
        form_vert[0] = m_win_elems[m_win_elems.size() - 1].unmapped_vert[0].vert_data;
        form_vert[1] = m_win_elems[m_win_elems.size() - 1].unmapped_vert[1].vert_data;
        form_vert[2] = m_win_elems[m_win_elems.size() - 1].unmapped_vert[2].vert_data;
        form_vert[3] = m_win_elems[m_win_elems.size() - 1].unmapped_vert[3].vert_data;

        // Add titlebar to window
        if(!(p_wi->fl_b & DENGUI_WINDOW_FLAG_NO_TITLEBAR)) {
            tmp_pos.first = -1.0f;
            tmp_pos.second = -1.0f;
            tmp_size.first = 2.0f;
            tmp_size.second = DENGUI_TITLEBAR_HEIGHT;
            m_win_elems.resize(m_win_elems.size() + 1);

            addRelUnmappedRec (
                tmp_pos, 
                tmp_size,
                true, 
                REC_ORIGIN_VERTEX_TOP_LEFT,
                *(deng_ObjColorData*) &p_wi->tc,
                form_vert,
                m_win_elems[m_win_elems.size() - 1].unmapped_vert,
                m_win_elems[m_win_elems.size() - 1].indices
            );

            m_win_elems[m_win_elems.size() - 1].parent_id = p_wi->id;
            m_win_elems[m_win_elems.size() - 1].child_id = DENGUI_TITLEBAR_ID;
            m_win_elems[m_win_elems.size() - 1].is_visible = true;
            m_win_elems[m_win_elems.size() - 1].is_interactive = false;
            m_win_elems[m_win_elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_UNMAPPED;
            m_win_elems[m_win_elems.size() - 1].onLMBClickFunc = NULL;
            m_win_elems[m_win_elems.size() - 1].onMMBClickFunc = NULL;
            m_win_elems[m_win_elems.size() - 1].onRMBClickFunc = NULL;
            m_win_elems[m_win_elems.size() - 1].onScrDownFunc = NULL;
            m_win_elems[m_win_elems.size() - 1].onScrUpFunc = NULL;
            

            // Add minimise and maximise triangles if needed
            if(!(p_wi->fl_b & DENGUI_WINDOW_FLAG_NO_COLLAPSE)) {
                tmp_pos.first = -1.0f + (2 * DENGUI_TITLEBAR_ELEM_MARGIN);
                tmp_pos.second = -1.0f + DENGUI_TITLEBAR_ELEM_MARGIN;
                tmp_size.first = DENGUI_TITLEBAR_HEIGHT - (2 * DENGUI_TITLEBAR_ELEM_MARGIN);
                tmp_size.second = DENGUI_TITLEBAR_HEIGHT - (2 * DENGUI_TITLEBAR_ELEM_MARGIN);

                m_win_elems.resize(m_win_elems.size() + 1);
                // Add minimizable triangle
                addGenTriangle (
                    tmp_pos, 
                    tmp_size,
                    rel_tri_pos,
                    true,
                    REC_ORIGIN_VERTEX_TOP_LEFT,
                    *(deng_ObjColorData*) &p_wi->sc,
                    form_vert,
                    m_win_elems[m_win_elems.size() - 1].unmapped_vert,
                    m_win_elems[m_win_elems.size() - 1].indices
                );

                m_win_elems[m_win_elems.size() - 1].parent_id = p_wi->id;
                m_win_elems[m_win_elems.size() - 1].child_id = DENGUI_MINIMISE_TRIANGLE_ID;
                m_win_elems[m_win_elems.size() - 1].is_visible = true;
                m_win_elems[m_win_elems.size() - 1].is_interactive = true;
                m_win_elems[m_win_elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_UNMAPPED;
                m_win_elems[m_win_elems.size() - 1].onLMBClickFunc = minTriangleCallback;
                m_win_elems[m_win_elems.size() - 1].onMMBClickFunc = NULL;
                m_win_elems[m_win_elems.size() - 1].onRMBClickFunc = NULL;
                m_win_elems[m_win_elems.size() - 1].onScrUpFunc = NULL;
                m_win_elems[m_win_elems.size() - 1].onScrDownFunc = NULL;

                // Add maximise triangle
                m_win_elems.resize(m_win_elems.size() + 1);
                switch (p_wi->wt)
                {
                case WINDOW_TYPE_FLOATING:
                case WINDOW_TYPE_STATIC_TOP:
                    rel_tri_pos[0] = {0.0f, -1.0f};
                    rel_tri_pos[1] = {1.0f, 1.0f};
                    rel_tri_pos[2] = {-1.0f, 1.0f};

                    addGenTriangle (
                        tmp_pos,
                        tmp_size,
                        rel_tri_pos,
                        true,
                        REC_ORIGIN_VERTEX_TOP_LEFT,
                        *(deng_ObjColorData*) &p_wi->sc,
                        form_vert,
                        m_win_elems[m_win_elems.size() - 1].unmapped_vert,
                        m_win_elems[m_win_elems.size() - 1].indices
                    );
                    break;
                
                case WINDOW_TYPE_STATIC_RIGHT:
                    rel_tri_pos[0] = {-1.0f, 0.0f};
                    rel_tri_pos[1] = {1.0f, -1.0f};
                    rel_tri_pos[2] = {1.0f, 1.0f};

                    addGenTriangle (
                        tmp_pos,
                        tmp_size,
                        rel_tri_pos,
                        true,
                        REC_ORIGIN_VERTEX_TOP_RIGHT,
                        *(deng_ObjColorData*) &p_wi->sc,
                        form_vert,
                        m_win_elems[m_win_elems.size() - 1].unmapped_vert,
                        m_win_elems[m_win_elems.size() - 1].indices
                    );
                    break; 
                
                case WINDOW_TYPE_STATIC_LEFT:
                    rel_tri_pos[0] = {-1.0f, -1.0f};
                    rel_tri_pos[1] = {1.0f, 0.0f};
                    rel_tri_pos[2] = {-1.0f, 1.0f};

                    addGenTriangle (
                        tmp_pos,
                        tmp_size,
                        rel_tri_pos,
                        true,
                        REC_ORIGIN_VERTEX_TOP_LEFT,
                        *(deng_ObjColorData*) &p_wi->sc,
                        form_vert,
                        m_win_elems[m_win_elems.size() - 1].unmapped_vert,
                        m_win_elems[m_win_elems.size() - 1].indices
                    );

                    break;

                default:
                    break;
                }

                m_win_elems[m_win_elems.size() - 1].parent_id = p_wi->id;
                m_win_elems[m_win_elems.size() - 1].child_id = DENGUI_MAXIMISE_TRIANGLE_ID;
                m_win_elems[m_win_elems.size() - 1].is_visible = false;
                m_win_elems[m_win_elems.size() - 1].is_interactive = true;
                m_win_elems[m_win_elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_UNMAPPED;
                m_win_elems[m_win_elems.size() - 1].onLMBClickFunc = maxTriangleCallback;
                m_win_elems[m_win_elems.size() - 1].onMMBClickFunc = NULL;
                m_win_elems[m_win_elems.size() - 1].onRMBClickFunc = NULL;
                m_win_elems[m_win_elems.size() - 1].onScrUpFunc = NULL;
                m_win_elems[m_win_elems.size() - 1].onScrDownFunc = NULL;
            }

            // Add close button if needed ([X])
            if(!(p_wi->fl_b & DENGUI_WINDOW_FLAG_NO_CLOSE)) {
                dengUtils::bitmapStr str;
                str.text = "[X]";
                tmp_pos.first = -1.0f + (2 * DENGUI_TITLEBAR_ELEM_MARGIN);
                tmp_pos.second = -1.0f + DENGUI_TITLEBAR_ELEM_MARGIN;
                
                m_win_elems.resize(m_win_elems.size() + 1);
                // Add closing button as a text box
                addText (
                    tmp_pos,
                    DENGUI_TITLEBAR_HEIGHT - (2 * DENGUI_TITLEBAR_ELEM_MARGIN),
                    draw_bounds,
                    REC_ORIGIN_VERTEX_TOP_RIGHT,
                    str,
                    {(unsigned char) (p_wi->sc.first * 255), (unsigned char) (p_wi->sc.second * 255), (unsigned char) (p_wi->sc.third * 255)},
                    form_vert,
                    m_win_elems[m_win_elems.size() - 1].mapped_vert,
                    m_win_elems[m_win_elems.size() - 1].indices,
                    m_win_elems[m_win_elems.size() - 1].texture,
                    m_win_elems[m_win_elems.size() - 1].tex_box
                );

                m_win_elems[m_win_elems.size() - 1].parent_id = p_wi->id;
                m_win_elems[m_win_elems.size() - 1].child_id = DENGUI_CLOSE_BTN_ID;
                m_win_elems[m_win_elems.size() - 1].is_visible = true;
                m_win_elems[m_win_elems.size() - 1].is_interactive = true;
                m_win_elems[m_win_elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_TEXTURE_MAPPED;
                m_win_elems[m_win_elems.size() - 1].onLMBClickFunc = closeBtnCallback;
                m_win_elems[m_win_elems.size() - 1].onMMBClickFunc = NULL;
                m_win_elems[m_win_elems.size() - 1].onRMBClickFunc = NULL;
                m_win_elems[m_win_elems.size() - 1].onScrUpFunc = NULL;
                m_win_elems[m_win_elems.size() - 1].onScrDownFunc = NULL;
            }
        }
    }


    // Window class getters 
    dengMath::vec4<float> Window::getPC() { return m_primary_color; }
    dengMath::vec4<float> Window::getSC() { return m_secondary_color; }
    dengMath::vec4<float> Window::getTC() { return m_tertiary_color; }

    std::vector<WindowElement> Window::getWindowElements() { return m_win_elems; }
    std::string Window::getId() { return m_id; }

    // Window class setters
    void Window::setPC(dengMath::vec4<float> *p_pc) { m_primary_color = *p_pc; }
    void Window::setSC(dengMath::vec4<float> *p_sc) { m_secondary_color = *p_sc; }
    void Window::setTC(dengMath::vec4<float> *p_tc) { m_tertiary_color = *p_tc; }
    void Window::setPos(dengMath::vec2<float> &pos) { m_pos = pos; }
    void Window::setSize(dengMath::vec2<float> &size) { m_size = size; }
    void Window::setMB(Menubar *p_mb) { m_p_mb = p_mb; };
}