#include "../../headers/deng/api_core.h" 
#include <string>

extern dengui::MouseInputInfo ext_mii;

namespace dengui {
    /***********************************/
    /********** EVENTS THREAD **********/
    /***********************************/

    /* Events constructor */
    Events::Events(EventInfo &ev_info) {
        m_info = ev_info;
        std::thread input_thread(&Events::inputPoll, this);
        input_thread.detach();
    }

    Events::~Events() {
        delete m_info.p_dc;
        delete m_info.p_ra;
    }


    /* Find triangle based collision */
    deng_bool_t Events::findCollision (
        dengMath::vec2<deng_vec_t> point, 
        WindowElement *p_elem
    ) {
        size_t l_index, r_index;
        bool is_colliding = false;
        dengMath::vec2<deng_vec_t> *vc;
        dengMath::vec2<deng_vec_t> *vn;

        // Check every triangle for potential collision
        switch (p_elem->color_mode)
        {
        case ELEMENT_COLOR_MODE_UNMAPPED:
            // printf("MOUSE: %f, %f\n", point.first, point.second);
            for(l_index = p_elem->col_vert_bounds.first; l_index < p_elem->col_vert_bounds.second; l_index++) {
                r_index = l_index + 1;
                if(r_index == p_elem->col_vert_bounds.second) 
                    r_index = p_elem->col_vert_bounds.first;
                
                vc = (dengMath::vec2<deng_vec_t>*) &p_elem->unmapped_vert[l_index].vert_data;
                vn = (dengMath::vec2<deng_vec_t>*) &p_elem->unmapped_vert[r_index].vert_data;

                if
                (
                    ((vc->second >= point.second && vn->second < point.second) ||
                    (vc->second < point.second && vn->second >= point.second)) &&
                    (point.first < (vn->first - vc->first) * (point.second - vc->second) / (vn->second - vc->second) + vc->first)
                ) is_colliding = !is_colliding;

            }
            break;

        case ELEMENT_COLOR_MODE_TEXTURE_MAPPED:
            for(l_index = p_elem->col_vert_bounds.first; l_index < p_elem->col_vert_bounds.second; l_index++) {
                r_index = l_index + 1;
                if(r_index == p_elem->col_vert_bounds.second) 
                    r_index = p_elem->col_vert_bounds.first;

                vc = (dengMath::vec2<deng_vec_t>*) &p_elem->mapped_vert[l_index].vert_data;
                vn = (dengMath::vec2<deng_vec_t>*) &p_elem->mapped_vert[r_index].vert_data;

                if
                (
                    ((vc->second >= point.second && vn->second < point.second) ||
                    (vc->second < point.second && vn->second >= point.second)) &&
                    (point.first < (vn->first - vc->first) * (point.second - vc->second) / (vn->second - vc->second) + vc->first)
                ) is_colliding = !is_colliding;

            }
            break;
        }

        return is_colliding;
    }


    /* Wait until no btn input is found */
    void Events::waitForNoInput(deng_MouseButton btn) {
        bool is_btn = true;
        deng_i32_t index;
        
        while(is_btn) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DENGUI_EV_INPUT_SLEEP_INTERVAL));
            
            // Check if btn is found
            is_btn = false;
            ext_mii.mut.lock();
            for(index = 0; index < ext_mii.active_btn_c; index++) {
                if(ext_mii.active_btn[index] == btn) {
                    is_btn = true;
                    break;
                }
            }
            ext_mii.mut.unlock();
        }
    }

    
    /* Main input polling method (seperate thread) */
    void Events::inputPoll() {
        size_t l_index; 
        deng_i32_t r_index;
        dengMath::vec2<deng_vec_t> vec_mouse;
        while(deng_IsRunning(m_p_win)) {
            m_info.p_res_mut->lock();
            // Check if collision with any element has happened and if click callback should be triggered
            for(l_index = 0; l_index < m_elem_infos.size(); l_index++) {
                std::lock_guard<std::mutex> lck(ext_mii.mut);

                vec_mouse.first = dengMath::Conversion::pixelSizeToVector2DSize (
                    (deng_px_t) ext_mii.mouse_coords.first,
                    m_info.deng_window_area,
                    DENG_COORD_AXIS_X
                ) - 1.0f;

                vec_mouse.second = dengMath::Conversion::pixelSizeToVector2DSize (
                    (deng_px_t) ext_mii.mouse_coords.second,
                    m_info.deng_window_area,
                    DENG_COORD_AXIS_Y
                ) - 1.0f;

                if
                (
                    m_elem_infos[l_index].is_visible &&
                    m_elem_infos[l_index].is_interactive && 
                    findCollision(vec_mouse, &m_elem_infos[l_index])
                ) {
                    // Search for active button clicks
                    for(r_index = 0; r_index < ext_mii.active_btn_c; r_index++) {
                        m_info.p_update_mut->lock();
                        switch (ext_mii.active_btn[r_index])
                        {
                        case DENG_MOUSE_BTN_1:
                            if(m_elem_infos[l_index].onLMBClickFunc) {
                                m_elem_infos[l_index].onLMBClickFunc (
                                    &m_elem_infos[l_index],
                                    this
                                );
                            }
                            break;

                        case DENG_MOUSE_BTN_2:
                            if(m_elem_infos[l_index].onMMBClickFunc)
                                m_elem_infos[l_index].onMMBClickFunc (
                                    &m_elem_infos[l_index],
                                    this
                                );
                            break;

                        case DENG_MOUSE_BTN_3:
                            if(m_elem_infos[l_index].onRMBClickFunc)
                                m_elem_infos[l_index].onRMBClickFunc (
                                    &m_elem_infos[l_index],
                                    this
                                );
                            break;

                        case DENG_MOUSE_SCROLL_DOWN:
                            if(m_elem_infos[l_index].onScrDownFunc)
                                m_elem_infos[l_index].onScrDownFunc (
                                    &m_elem_infos[l_index],
                                    this
                                );
                            break;

                        case DENG_MOUSE_SCROLL_UP:
                            if(m_elem_infos[l_index].onScrUpFunc)
                                m_elem_infos[l_index].onScrUpFunc (
                                    &m_elem_infos[l_index],
                                    this
                                );
                            break;
                        
                        default:
                            break;
                        }

                        lck.~lock_guard();
                        m_info.p_update_mut->unlock();
                        waitForNoInput(ext_mii.active_btn[r_index]);
                    }
                }
            }
            m_info.p_res_mut->unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(DENGUI_ITERATION_SLEEP_INTERVAL));
        }
    }


    /* Remove removed elements from asset vector */
    void Events::windowAssetSync() {
        deng_i32_t l_index, r_index;

        // Find all removed window assets names (O(n^2))
        for(l_index = 0; l_index < (deng_i32_t) m_window_names.size(); l_index++) {
            deng_bool_t found_elem = false;
            for(r_index = 0; r_index < (deng_i32_t) m_elem_infos.size(); r_index++) {
                if(m_elem_infos[r_index].parent_id == m_window_names[l_index]) {
                    found_elem = true;
                    break;
                }
            }

            if(found_elem) {
                for(r_index = 0; r_index < (deng_i32_t) m_info.p_assets->size(); r_index++) {
                    if 
                    (
                        strlen((*m_info.p_assets)[r_index].id) > m_window_names[l_index].size() + 1 &&
                        !strcmp (
                            (*m_info.p_assets)[r_index].id + 
                            ( 
                                strlen((*m_info.p_assets)[r_index].id) -
                                m_window_names[l_index].size()
                            ), 
                            m_window_names[l_index].c_str()
                        )
                    ) {
                        m_info.p_assets->erase(m_info.p_assets->begin() + r_index);
                        r_index--;
                        if(r_index < 0) r_index = 0;
                    }
                }
            }
        }
    }


    /* Push new window elements to elements vector */
    /* Only element vector with elements appliable for the same window can be used */ 
    void Events::pushWindowElements (
        std::vector<WindowElement> elems,
        deng_bool_t update_buffers
    ) {
        size_t l_index, r_index, tex_index;
        m_elem_mut.lock();
        m_info.p_res_mut->lock(); 

        // Resize assets to elements size
        l_index = m_info.p_assets->size();
        m_info.p_assets->resize(m_info.p_assets->size() + elems.size());
        
        // Check if new window name is present
        deng_bool_t is_prev_win = false;
        for(r_index = 0; r_index < m_window_names.size(); r_index++) {
            if(!strcmp(elems[0].parent_id.c_str(), m_window_names[r_index].c_str())) {
                is_prev_win = true;
                break;
            }
        }

        if(!is_prev_win) {
            m_window_names.push_back(elems[0].parent_id);
            LOG("New UI window: " + std::string(m_window_names[m_window_names.size() - 1]));
        }

        tex_index = 0;
        for(r_index = 0; r_index < elems.size(); l_index++, r_index++) {
            // Create asset name
            elems[r_index].asset_id = (char*) calloc (
                elems[r_index].child_id.size() + elems[r_index].parent_id.size() + 5,
                sizeof(char)
            );

            sprintf ( 
                elems[r_index].asset_id,
                "X: %s#%s",
                elems[r_index].parent_id.c_str(),
                elems[r_index].child_id.c_str()
            );

            // Base asset information
            (*m_info.p_assets)[l_index].id = elems[r_index].asset_id;
            (*m_info.p_assets)[l_index].is_shown = elems[r_index].is_visible;
            (*m_info.p_assets)[l_index].indices.size = elems[r_index].indices.size();
            (*m_info.p_assets)[l_index].indices.p_indices = (deng_ui32_t*) calloc (
                elems[r_index].indices.size(),
                sizeof(deng_ui32_t)
            );

            memcpy (
                (*m_info.p_assets)[l_index].indices.p_indices,
                elems[r_index].indices.data(),
                elems[r_index].indices.size() * sizeof(deng_ui32_t)
            );

            switch (elems[r_index].color_mode)
            {
            case ELEMENT_COLOR_MODE_TEXTURE_MAPPED:
                // Add new TextureImageData instance to its vector
                tex_index = m_info.p_textures->size();
                m_info.p_textures->resize(tex_index + 1);
                (*m_info.p_textures)[tex_index].texture.pixel_data.width = elems[r_index].tex_box.first;
                (*m_info.p_textures)[tex_index].texture.pixel_data.height = elems[r_index].tex_box.second;
                (*m_info.p_textures)[tex_index].texture.pixel_data.size = elems[r_index].texture.size();
                (*m_info.p_textures)[tex_index].texture.id = elems[r_index].asset_id;
                
                (*m_info.p_textures)[tex_index].texture.pixel_data.p_pixel_data = (deng_ui8_t*) calloc (
                    (*m_info.p_textures)[tex_index].texture.pixel_data.size,
                    sizeof(deng_ui8_t)
                );

                memcpy (
                    (*m_info.p_textures)[tex_index].texture.pixel_data.p_pixel_data,
                    elems[r_index].texture.data(),
                    elems[r_index].texture.size() * sizeof(deng_ui8_t)
                );

                (*m_info.p_assets)[l_index].tex_id = elems[r_index].asset_id;
                (*m_info.p_assets)[l_index].asset_mode = DENG_ASSET_MODE_2D_TEXTURE_MAPPED;
                (*m_info.p_assets)[l_index].vertices.size = elems[r_index].mapped_vert.size();
                (*m_info.p_assets)[l_index].vertices.p_tex_mapped_vert_data_2d = (VERT_MAPPED_2D*) calloc (
                    (*m_info.p_assets)[l_index].vertices.size,
                    sizeof(VERT_MAPPED_2D)
                );

                memcpy (
                    (*m_info.p_assets)[l_index].vertices.p_tex_mapped_vert_data_2d,
                    elems[r_index].mapped_vert.data(),
                    elems[r_index].mapped_vert.size() * sizeof(VERT_MAPPED_2D)
                );
                break;
            
            case ELEMENT_COLOR_MODE_UNMAPPED:
                (*m_info.p_assets)[l_index].asset_mode = DENG_ASSET_MODE_2D_UNMAPPED;
                (*m_info.p_assets)[l_index].vertices.size = elems[r_index].unmapped_vert.size();

                (*m_info.p_assets)[l_index].vertices.p_unmapped_vert_data_2d = (VERT_UNMAPPED_2D*) calloc (
                    elems[r_index].unmapped_vert.size(),
                    sizeof(VERT_UNMAPPED_2D)
                );

                memcpy (
                    (*m_info.p_assets)[l_index].vertices.p_unmapped_vert_data_2d,
                    elems[r_index].unmapped_vert.data(),
                    elems[r_index].unmapped_vert.size() * sizeof(VERT_UNMAPPED_2D)
                );
                break;
            
            default:
                break;
            }
        }

        // Insert elements to element vector
        m_elem_infos.insert (
            m_elem_infos.end(),
            elems.begin(),
            elems.end()
        );

        // Check if buffers need to be reallocated 
        if(update_buffers)
            allocateNewResources();
            
        m_info.p_res_mut->unlock();
        m_elem_mut.unlock();
    }

    
    /* Update all assets visibility */
    void Events::updateAssetsVisibility(std::string parent_id) {
        size_t l_index, r_index;
        std::vector<WindowElement> tmp_we;
        
        // Find all current window elements
        for(l_index = 0; l_index < m_elem_infos.size(); l_index++) {
            if (
                !strncmp (
                    m_elem_infos[l_index].parent_id.c_str(),
                    parent_id.c_str(),
                    parent_id.size()
                )
            ) tmp_we.push_back(m_elem_infos[l_index]);
        }

        // Update visibilities
        size_t asset_i = 0;
        for(l_index = 0; l_index < tmp_we.size(); l_index++) {
            if 
            (
                strcmp (
                    (*m_info.p_assets)[asset_i].id,
                    tmp_we[l_index].asset_id
                )
            ) {
                for(r_index = 0; r_index < m_info.p_assets->size(); r_index++) {
                    if(!strcmp((*m_info.p_assets)[r_index].id, tmp_we[l_index].asset_id)) {
                        asset_i = r_index;
                        break;
                    } 
                }
            }

            (*m_info.p_assets)[asset_i].is_shown = tmp_we[l_index].is_visible;
        }
    }


    /* Allocate new resources according to the window element changes and previous assets */
    void Events::allocateNewResources() {
        m_info.p_dc->mkCommandPool(m_info.device);
        
        m_info.p_ra->mkBuffers (
            m_info.device,
            m_info.gpu,
            *m_info.p_dc->getComPool(),
            m_info.g_queue
        );

        m_info.p_dc->recordDrawCommands (
            m_info.device,
            m_info.g_queue,
            m_info.renderpass,
            m_info.extent,
            m_info.background,
            m_info.p_ra->getBD()
        );
    }


    /* Get all window elements from event class             *
     * These methods are made to be used from main thread   */
    std::vector<WindowElement> *Events::getElems() { return &m_elem_infos; }
    std::vector<VkCommandBuffer> *Events::getCmdBuffers() { return m_info.p_dc->getComBufs(); }
    VkCommandPool *Events::getCmdPool() { return m_info.p_dc->getComPool(); }
    VkBuffer *Events::getMainBuffer() { return m_info.p_ra->getBD().p_main_buffer; }
    VkDeviceMemory *Events::getMainBufMem() { return m_info.p_ra->getBD().p_main_buffer_memory; }
    void Events::setUpdateFlag() { *m_info.p_update_flag = true; }


    /************************************/
    /************************************/
    /****** CALLBACK FUNCTIONS **********/
    /************************************/
    /************************************/

    /* Base function for minimising window */
    void minTriangleCallback (
        WindowElement *p_cur_elem,
        Events *p_ev
    ) {
        size_t l_index;
        std::vector<WindowElement> *p_elems;
        p_elems = p_ev->getElems();

        // Hide all elements exept maximise triangle
        for(l_index = 0; l_index < p_elems->size(); l_index++) {
            LOG("CUR_PARENT: " + std::string(p_cur_elem->parent_id));
            LOG("ELEM_PARENT: " + std::string((*p_elems)[l_index].parent_id));
            if 
            (
                !strcmp (
                    (*p_elems)[l_index].child_id.c_str(),
                    DENGUI_MAXIMISE_TRIANGLE_ID   
                ) &&
                (*p_elems)[l_index].parent_id == p_cur_elem->parent_id
            ) {
                (*p_elems)[l_index].is_visible = true;
                LOG("Enabled " + std::string((*p_elems)[l_index].child_id));
            }

            else if (
                !strncmp (
                    (*p_elems)[l_index].parent_id.c_str(), 
                    p_cur_elem->parent_id.c_str(),
                    p_cur_elem->parent_id.size()
                )
            ) {
                LOG("Disabled " + std::string((*p_elems)[l_index].child_id));
                (*p_elems)[l_index].is_visible = false;
            }
            
            printf (
                "CMP_RES: %d\n",
                strncmp (
                    (*p_elems)[l_index].parent_id.c_str(), 
                    p_cur_elem->parent_id.c_str(),
                    p_cur_elem->parent_id.size()
                )
            );
        }

        p_ev->updateAssetsVisibility(p_cur_elem->parent_id);
        p_ev->allocateNewResources();
        p_ev->setUpdateFlag();
    }

    void closeBtnCallback (
        WindowElement *p_cur_elem,
        Events *p_ev
    ) {
        deng_i32_t l_index;
        std::vector<WindowElement> *p_elems;
        p_elems = p_ev->getElems();

        /* Remove all window elements */
        for(l_index = 0; l_index < (deng_i32_t) p_elems->size(); l_index++) {
            if(p_elems->at(l_index).parent_id == p_cur_elem->parent_id) {
                p_elems->erase(p_elems->begin() + l_index);
                l_index--;
                if(l_index < 0) l_index = 0;
            }
        }

        p_ev->windowAssetSync();
        p_ev->allocateNewResources();
        p_ev->setUpdateFlag();
    }

    void maxTriangleCallback (
        WindowElement *p_cur_elem,
        Events *p_ev
    ) {
        LOG("maximising...");
        size_t l_index;
        std::vector<WindowElement> *p_elems;
        p_elems = p_ev->getElems();

        /* Hide all elements exept maximise triangle */
        for(l_index = 0; l_index < p_elems->size(); l_index++) {
            if 
            (
                !strcmp (
                    (*p_elems)[l_index].child_id.c_str(),
                    DENGUI_MAXIMISE_TRIANGLE_ID   
                ) &&
                (*p_elems)[l_index].parent_id == p_cur_elem->parent_id
            ) (*p_elems)[l_index].is_visible = false;

            else if (
                !strncmp (
                    (*p_elems)[l_index].parent_id.c_str(),
                    p_cur_elem->parent_id.c_str(),
                    p_cur_elem->parent_id.size()
                )
            ) (*p_elems)[l_index].is_visible = true;
        }

        p_ev->updateAssetsVisibility(p_cur_elem->parent_id);
        p_ev->allocateNewResources();
        p_ev->setUpdateFlag();
    }
}
