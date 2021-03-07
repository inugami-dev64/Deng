#define __DENGUI_EVENTS_CPP
#include <dengui/dengui_events.h>

extern dengui::MouseInputInfo ext_mii;
namespace dengui {
    
    /*************************************/
    /******* ELEMENT HANDLER CLASS *******/
    /*************************************/
    __EH::__EH (
        std::vector<deng_Asset> *p_assets, 
        std::vector<deng::vulkan::TextureImage> *p_tex,
        vulkan::__FrameUpdater *p_vk_frame_upd,
        Events *p_ev,
        deng_bool_t *p_fl_flag,
        deng::vulkan::InstanceCreator *p_ic,
        deng::vulkan::DescriptorCreator *p_desc_c,
        std::mutex *p_res_mut
    ) {
        m_fi.p_float_flag = p_fl_flag;
        m_p_vk_frame_upd = p_vk_frame_upd;
        m_p_assets = p_assets;
        m_p_tex = p_tex;
        m_p_res_mut = p_res_mut;
        m_p_ic = p_ic;
        m_p_desc_c = p_desc_c;
        m_mouse_transform.row1 = {1.0f, 0.0f, 0.0f};
        m_mouse_transform.row2 = {0.0f, 1.0f, 0.0f};
        m_mouse_transform.row3 = {0.0f, 0.0f, 1.0f};
    }


    /* 
     * Find WindowData vector index from string window_id
     * If there is no WindowData instance with specified window_id then
     * vector will be resized and new WindowData instance created
     */
    size_t __EH::__findWindowIndex (
        const std::string &window_id, 
        size_t pref_win_i,
        deng_bool_t new_win
    ) {
        size_t i, win_i = 0;
        size_t same_win_c = 0;
        size_t last_win_i = 0;

        deng_bool_t found_win = false;
        for(i = 0; i < m_win_data.size(); i++) {
            if(!strncmp(m_win_data[i].window_id.c_str(), window_id.c_str(), window_id.size())) {
                win_i = i;
                found_win = true;
                char *tmp = (char*) m_win_data[i].window_id.c_str() + m_win_data[i].window_id.size() - 1;
                while(tmp >= m_win_data[i].window_id.c_str()) {
                    if(*tmp > '9' || *tmp < '0') break;
                    tmp--;
                }

                tmp++;
                if(m_win_data[i].window_id.c_str() - tmp && atol(tmp) && last_win_i < (size_t) atol(tmp))
                    last_win_i = atol(tmp);
                same_win_c++;
            }
        }

        if(!found_win || new_win) {
            win_i = m_win_data.size();
            m_win_data.resize(m_win_data.size() + 1);
            if(same_win_c) {
                m_win_data[win_i].window_id = window_id + (
                    same_win_c > last_win_i ? std::to_string(same_win_c + 1) : std::to_string(last_win_i + 1)
                );
            }
            else 
                m_win_data[win_i].window_id = window_id;
            m_win_data[win_i].asset_bounds.first = m_p_assets->size();
            m_win_data[win_i].asset_bounds.second = m_p_assets->size();

            m_win_data[win_i].tex_bounds.first = m_p_tex->size();
            m_win_data[win_i].tex_bounds.second = m_p_tex->size();
        }

        // Search again for existing windows but this time according to prefered index
        else if(!new_win && same_win_c && pref_win_i) {
            char f_win_id[512];
            memset(f_win_id, 0, 512);
            sprintf (
                f_win_id,
                "%s%ld",
                window_id.c_str(),
                pref_win_i 
            );

            found_win = false;
            for(i = 0; i < m_win_data.size(); i++) {
                if(!strncmp(m_win_data[i].window_id.c_str(), f_win_id, strlen(f_win_id))) {
                    win_i = i;
                    found_win = true;
                    break;
                }
            }

            if(!found_win) {
                win_i = m_win_data.size();
                m_win_data.resize(m_win_data.size() + 1);
                m_win_data[win_i].window_id = window_id + (
                    same_win_c > last_win_i ? std::to_string(same_win_c + 1) : std::to_string(last_win_i + 1)
                );
                
                m_win_data[win_i].asset_bounds.first = m_p_assets->size();
                m_win_data[win_i].asset_bounds.second = m_p_assets->size();

                m_win_data[win_i].tex_bounds.first = m_p_tex->size();
                m_win_data[win_i].tex_bounds.second = m_p_tex->size();
            }
        }

        return win_i;
    }


    
    /*
     * Find cursor drop down menu index 
     * This method is a bit different from __findWindowIndex() method since pushing drop down menu
     * elements always creates new drop down menu instance
     */
    size_t __EH::__findCursorDDMIndex(const std::string &ddm_id) {
        size_t same_id_c = 0;
        size_t max_id_num = 0;

        size_t out_i = 0;
        for(size_t i = 0; i < m_cur_ddm_infos.size(); i++) {
            if(!strncmp(m_cur_ddm_infos[i].ddm_id.c_str(), ddm_id.c_str(), ddm_id.size())) {
                same_id_c++;
                if(m_cur_ddm_infos[i].ddm_id.size() > ddm_id.size()) {
                    char *tmp = (char*) m_cur_ddm_infos[i].ddm_id.c_str() + m_cur_ddm_infos[i].ddm_id.size() - 1;
                    while(tmp >= m_cur_ddm_infos[i].ddm_id.c_str() && *tmp >= '0' && *tmp <= '9') 
                        tmp--;

                    if(tmp == m_cur_ddm_infos[i].ddm_id.c_str() && *tmp >= '0' && *tmp <= '9')
                        continue;

                    tmp++;
                    if(atol(tmp) && max_id_num < (size_t) atol(tmp))
                        max_id_num = (size_t) atol(tmp);
                }
            }
        }

        out_i = m_cur_ddm_infos.size();
        m_cur_ddm_infos.resize(out_i + 1);
        if(same_id_c) {
            m_cur_ddm_infos[out_i].ddm_id = ddm_id + (
                same_id_c > max_id_num ? std::to_string(same_id_c + 1) : std::to_string(max_id_num + 1)
            );
        }
        else m_cur_ddm_infos[out_i].ddm_id = ddm_id;

        return out_i;
    }



    /*
     * Initialise floating mode for window assets
     */
    void __EH::__initFloating (
        dengMath::vec2<deng_ui32_t> asset_bounds,
        dengMath::vec2<deng_vec_t> mouse_pos
    ) {
        m_mouse_transform.row1.third = mouse_pos.first;
        m_mouse_transform.row2.third = mouse_pos.second;
        m_fi.asset_bounds = asset_bounds;
        m_fi.fl_delta_transform.row1 = {1.0f, 0.0f, 0.0f};
        m_fi.fl_delta_transform.row2 = {0.0f, 1.0f, 0.0f};
        m_fi.fl_delta_transform.row3 = {0.0f, 0.0f, 1.0f};
    }


    /*
     * Check if drop down menu should be spawn
     */
    deng_bool_t __EH::__EH_CheckDDMSpawn(dengMath::vec2<deng_vec_t> mouse_pos) {
        size_t i;
        deng_bool_t spawn_menu = false;

        m_mouse_transform.row1.third = mouse_pos.first;
        m_mouse_transform.row2.third = mouse_pos.second;

        // Search for appropriate button stroke 
        spawn_menu = deng_FindKeyStatus (
            DENG_KEY_UNKNOWN, 
            DENGUI_DEFAULT_DROP_DOWN_MENU_TRIGGER, 
            DENG_INPUT_TYPE_MOUSE, 
            DENG_INPUT_EVENT_TYPE_ACTIVE
        );

        // Check if any drag down menus should be displayed
        if(spawn_menu) {
            for(i = 0; i < m_cur_ddm_infos.size(); i++) {
                if
                (
                    findPtCollision (
                        mouse_pos, 
                        m_cur_ddm_infos[i].active_verts, 
                        m_cur_ddm_infos[i].active_vert_c
                    )
                ) {
                    // Drop down menu bounds check
                    if(m_mouse_transform.row1.third > (1.0f - m_cur_ddm_infos[i].ddm_size.first))
                        m_mouse_transform.row1.third = 1.0f - m_cur_ddm_infos[i].ddm_size.first;
                    if(m_mouse_transform.row2.third > 1.0f - (m_cur_ddm_infos[i].ddm_size.second))
                        m_mouse_transform.row2.third = 1.0f - m_cur_ddm_infos[i].ddm_size.second;

                    dengMath::Transformer::apply2DModelMatrix (
                        m_cur_ddm_infos[i].asset_bounds,
                        m_p_assets,
                        m_mouse_transform
                    );
                    
                    m_p_vk_frame_upd->lockFrame();
                    m_p_vk_frame_upd->updateVerts(m_cur_ddm_infos[i].asset_bounds);
                    m_p_vk_frame_upd->unlockFrame();
                    toggleVisibility (
                        m_cur_ddm_infos[i].asset_bounds,
                        &m_cur_ddm_infos[i].is_visible
                    );
                    LOG("Menu spawned");
                }
            }
        }
        
        return spawn_menu;
    }


    /* 
     * Check if any callback events on drop down menus should be triggered 
     * and if needed close drop down menu
     */
    deng_bool_t __EH::__EH_UpdateDDMEv (
        dengMath::vec2<deng_vec_t> mouse_pos, 
        Events *p_ev,
        deng_bool_t *p_ddm_flag
    ) {
        size_t i, j;
        std::array<deng_ObjVertData2D, 4> verts;

        // Check each drop menu instance for closing if needed 
        DDMCallback callback = NULL;
        deng_bool_t is_btn = false;
        for(i = 0; i < m_cur_ddm_infos.size(); i++) {
            if(!m_cur_ddm_infos[i].is_visible)
                continue;

            // Check for collision with each element 
            for(j = m_cur_ddm_infos[i].asset_bounds.first; j < m_cur_ddm_infos[i].asset_bounds.second; j++) {
                is_btn = false;
                if (
                    findPtCollision (
                        mouse_pos, 
                        &m_p_assets->at(j),
                        {0, 4}
                    )
                ) {
                    size_t elem_i = j - m_cur_ddm_infos[i].asset_bounds.first;
                    callback = NULL;
                    if(deng_FindKeyStatus(DENG_KEY_UNKNOWN, DENG_MOUSE_BTN_1, DENG_INPUT_TYPE_MOUSE, DENG_INPUT_EVENT_TYPE_ACTIVE)) {
                        callback = m_cur_ddm_infos[i].elems[elem_i].onLMBClickFunc;
                        is_btn = true;
                    }

                    else if(deng_FindKeyStatus(DENG_KEY_UNKNOWN, DENG_MOUSE_BTN_2, DENG_INPUT_TYPE_MOUSE, DENG_INPUT_EVENT_TYPE_ACTIVE)) {
                        callback = m_cur_ddm_infos[i].elems[elem_i].onMMBClickFunc;
                        is_btn = true;
                    }

                    else if(deng_FindKeyStatus(DENG_KEY_UNKNOWN, DENG_MOUSE_BTN_3, DENG_INPUT_TYPE_MOUSE, DENG_INPUT_EVENT_TYPE_ACTIVE)) {
                        callback = m_cur_ddm_infos[i].elems[elem_i].onRMBClickFunc;
                        is_btn = true;
                    }

                    if(callback) {
                        m_p_res_mut->unlock();
                        callback (
                            &m_cur_ddm_infos[i].elems[elem_i],
                            p_ev
                        );
                        *p_ddm_flag = false;
                        m_p_res_mut->lock();
                        dengMath::mat3<deng_vec_t> inv_mat = m_mouse_transform.inv();
                        dengMath::Transformer::apply2DModelMatrix (
                            m_cur_ddm_infos[i].asset_bounds,
                            m_p_assets,
                            inv_mat
                        );

                        toggleVisibility (
                            m_cur_ddm_infos[i].asset_bounds, 
                            &m_cur_ddm_infos[i].is_visible
                        );
                        return true;
                    }
                }

                if(is_btn) {
                    if(m_cur_ddm_infos[i].asset_bounds.second - m_cur_ddm_infos[i].asset_bounds.first > 1) {
                        verts[0] = (*m_p_assets)[m_cur_ddm_infos[i].asset_bounds.first].vertices.p_unmapped_vert_data_2d[0].vert_data;
                        verts[1] = (*m_p_assets)[m_cur_ddm_infos[i].asset_bounds.first].vertices.p_unmapped_vert_data_2d[1].vert_data;
                        verts[2] = (*m_p_assets)[m_cur_ddm_infos[i].asset_bounds.second - 2].vertices.p_unmapped_vert_data_2d[2].vert_data;
                        verts[3] = (*m_p_assets)[m_cur_ddm_infos[i].asset_bounds.second - 2].vertices.p_unmapped_vert_data_2d[3].vert_data;
                    }
                    else {
                        verts[0] = (*m_p_assets)[m_cur_ddm_infos[i].asset_bounds.first].vertices.p_unmapped_vert_data_2d[0].vert_data;
                        verts[1] = (*m_p_assets)[m_cur_ddm_infos[i].asset_bounds.first].vertices.p_unmapped_vert_data_2d[1].vert_data;
                        verts[2] = (*m_p_assets)[m_cur_ddm_infos[i].asset_bounds.first].vertices.p_unmapped_vert_data_2d[2].vert_data;
                        verts[3] = (*m_p_assets)[m_cur_ddm_infos[i].asset_bounds.first].vertices.p_unmapped_vert_data_2d[3].vert_data;
                    }

                    // Check for collision
                    if(!findPtCollision(mouse_pos, verts.data(), verts.size())) {
                        dengMath::mat3<deng_vec_t> inv_mat = m_mouse_transform.inv();
                        dengMath::Transformer::apply2DModelMatrix (
                            m_cur_ddm_infos[i].asset_bounds,
                            m_p_assets,
                            inv_mat
                        );

                        toggleVisibility (
                            m_cur_ddm_infos[i].asset_bounds, 
                            &m_cur_ddm_infos[i].is_visible
                        );

                        *p_ddm_flag = false;
                    } 
                }
            }
        }

        return false;
    }


    /*
     * Update ui windows and trigger callbacks if needed
     */
    deng_bool_t __EH::__EH_UpdateWindows (
        dengMath::vec2<deng_vec_t> mouse_pos, 
        Events *p_ev,
        deng_MouseButton &feedback
    ) {
        size_t win_i, i;
        // Check if collision with any window element has happened and if click callback should be triggered
        for(win_i = 0; win_i < m_win_data.size(); win_i++) {
            for(i = 0; i < m_win_data[win_i].win_elems.size(); i++) {
                if(!m_win_data[win_i].win_elems[i].is_interactive && !m_win_data[win_i].win_elems[i].is_drag_point)
                    continue;

                // Generic element collision detection
                if
                (
                    m_p_assets->at(m_win_data[win_i].asset_bounds.first + i).is_shown &&
                    findPtCollision (
                        mouse_pos, 
                        &(*m_p_assets)[m_win_data[win_i].asset_bounds.first + i], 
                        m_win_data[win_i].win_elems[i].col_vert_bounds
                    )
                ) {
                    // Check if hover callback should be called
                    if(m_win_data[win_i].win_elems[i].onHoverFunc) {
                        m_win_data[win_i].win_elems[i].onHoverFunc (
                            &m_win_data[win_i].win_elems[i],
                            p_ev
                        );
                    }

                    // Search for active button clicks
                    WindowCallback callback;
                    callback = NULL;
                    if(deng_FindKeyStatus(DENG_KEY_UNKNOWN, DENG_MOUSE_BTN_1, DENG_INPUT_TYPE_MOUSE, DENG_INPUT_EVENT_TYPE_ACTIVE)) 
                        callback = m_win_data[win_i].win_elems[i].onLMBClickFunc;

                    else if(deng_FindKeyStatus(DENG_KEY_UNKNOWN, DENG_MOUSE_BTN_2, DENG_INPUT_TYPE_MOUSE, DENG_INPUT_EVENT_TYPE_ACTIVE))
                        callback = m_win_data[win_i].win_elems[i].onMMBClickFunc;

                    else if(deng_FindKeyStatus(DENG_KEY_UNKNOWN, DENG_MOUSE_BTN_3, DENG_INPUT_TYPE_MOUSE, DENG_INPUT_EVENT_TYPE_ACTIVE))
                        callback = m_win_data[win_i].win_elems[i].onRMBClickFunc;

                    else if(deng_FindKeyStatus(DENG_KEY_UNKNOWN, DENG_MOUSE_SCROLL_DOWN, DENG_INPUT_TYPE_MOUSE, DENG_INPUT_EVENT_TYPE_ACTIVE))
                        callback = m_win_data[win_i].win_elems[i].onScrDownFunc;

                    else if(deng_FindKeyStatus(DENG_KEY_UNKNOWN, DENG_MOUSE_SCROLL_UP, DENG_INPUT_TYPE_MOUSE, DENG_INPUT_EVENT_TYPE_ACTIVE))
                        callback = m_win_data[win_i].win_elems[i].onScrUpFunc;

                    if(callback) {
                        m_p_res_mut->unlock();
                        callback (
                            &m_win_data[win_i].win_elems[i],
                            p_ev
                        );

                        m_p_res_mut->lock();
                        return true;
                    }

                    else if (
                        m_win_data[win_i].win_elems[i].is_drag_point && 
                        deng_FindKeyStatus(DENG_KEY_UNKNOWN, DENGUI_DEFAULT_DRAG_BTN, DENG_INPUT_TYPE_MOUSE, DENG_INPUT_EVENT_TYPE_ACTIVE)
                    ) {
                        __initFloating (
                            m_win_data[win_i].asset_bounds, 
                            mouse_pos
                        );
                        *m_fi.p_float_flag = true;
                    }
                }
            }
        }

        return false;
    }


    /*
     * Update window location transform
     * This method is meant to be used to update window location when dragging it
     */
    void __EH::__EH_UpdateFloating(dengMath::vec2<deng_vec_t> mouse_pos) {
        // Check if dragging button is still active
        deng_bool_t is_btn = deng_FindKeyStatus (
            DENG_KEY_UNKNOWN,
            DENGUI_DEFAULT_DRAG_BTN,
            DENG_INPUT_TYPE_MOUSE,
            DENG_INPUT_EVENT_TYPE_ACTIVE
        );

        if(!is_btn) {
            *m_fi.p_float_flag = false;
            return;
        }

        // Update transformation matrix
        m_fi.fl_delta_transform.row1.third = mouse_pos.first - m_mouse_transform.row1.third;
        m_fi.fl_delta_transform.row2.third = mouse_pos.second - m_mouse_transform.row2.third;
        m_mouse_transform.row1.third = mouse_pos.first;
        m_mouse_transform.row2.third = mouse_pos.second;

        dengMath::Transformer::apply2DModelMatrix( 
            m_fi.asset_bounds,
            m_p_assets,
            m_fi.fl_delta_transform
        );

        m_p_vk_frame_upd->lockFrame();
        m_p_vk_frame_upd->updateVerts(m_fi.asset_bounds);
        m_p_vk_frame_upd->unlockFrame();
    }


    /*
     * Find window instance by id
     */
    WindowData *__EH::__EH_FindWindow(const std::string &win_id) {
        for(size_t i = 0; i < m_win_data.size(); i++) {
            if(m_win_data[i].window_id == win_id)
                return &m_win_data[i];
        }

        return NULL;
    }


    /* Getter methods */
    std::vector<CursorDDMInfo> *__EH::__EH_GetDDMInfos() { return &m_cur_ddm_infos; }
    std::vector<WindowData> *__EH::__EH_GetWinData() { return &m_win_data; }


    /* 
     * Push new window elements to elements vector 
     * Only element vector with elements appliable for the same window can be used 
     */ 
    void __EH::pushWindowElements (
        const std::vector<WindowElement> &elems,
        std::vector<std::string> *p_hidden_children,
        const std::string &window_id,
        size_t pref_win_i,
        deng_bool_t update_buffers,
        deng_bool_t new_win
    ) {
        size_t i, j, win_i = __findWindowIndex (
            window_id,
            pref_win_i,
            new_win
        );
        m_p_res_mut->lock(); 

        size_t elem_offset = m_win_data[win_i].win_elems.size();
        m_win_data[win_i].win_elems.insert (
            m_win_data[win_i].win_elems.end(),
            elems.begin(),
            elems.end()
        );

        // Make window_id to element parent id 
        for(i = elem_offset; i < m_win_data[win_i].win_elems.size(); i++)
            m_win_data[win_i].win_elems[i].parent_id = m_win_data[win_i].window_id;

        // Windows are always visible by default
        m_win_data[win_i].is_visible = true;

        // Resize assets to elements size
        std::vector<deng_Asset> tmp_assets(elems.size());
        
        for(i = 0, j = elem_offset; i < elems.size(); i++, j++) {
            // Create asset name
            m_win_data[win_i].win_elems[j].asset_id = (char*) calloc (
                m_win_data[win_i].win_elems[j].child_id.size() + m_win_data[win_i].win_elems[j].parent_id.size() + 2,
                sizeof(char)
            );
            
            if(m_win_data[win_i].win_elems[j].child_id != "") {
                sprintf ( 
                    m_win_data[win_i].win_elems[j].asset_id,
                    "%s#%s",
                    m_win_data[win_i].win_elems[j].parent_id.c_str(),
                    m_win_data[win_i].win_elems[j].child_id.c_str()
                );
            }

            else {
                strcpy (
                    m_win_data[win_i].win_elems[j].asset_id, 
                    m_win_data[win_i].win_elems[j].parent_id.c_str()
                );
            }

            LOG("Window asset: " + std::string(m_win_data[win_i].win_elems[j].asset_id));
            // Base asset information
            tmp_assets[i].id = m_win_data[win_i].win_elems[j].asset_id;
            tmp_assets[i].indices.size = m_win_data[win_i].win_elems[j].indices.size();
            tmp_assets[i].indices.p_indices = m_win_data[win_i].win_elems[j].indices.data();
            tmp_assets[i].is_shown = true;
            if(p_hidden_children) {
                for(size_t hidden_i = 0; hidden_i < p_hidden_children->size(); hidden_i++) {
                    if(p_hidden_children->at(hidden_i) == m_win_data[win_i].win_elems[j].child_id)
                        tmp_assets[i].is_shown = false;
                }
            }

            switch (m_win_data[win_i].win_elems[j].color_mode)
            {
            case ELEMENT_COLOR_MODE_TEXTURE_MAPPED: {
                // Add new vulkan::TextureImage instance to its vector
                deng::vulkan::TextureImage tmp_img;
                tmp_img.texture.pixel_data.width = m_win_data[win_i].win_elems[j].tex_box.first;
                tmp_img.texture.pixel_data.height = m_win_data[win_i].win_elems[j].tex_box.second;
                tmp_img.texture.pixel_data.size = m_win_data[win_i].win_elems[j].texture.size();
                tmp_img.texture.pixel_data.p_pixel_data = m_win_data[win_i].win_elems[j].texture.data();
                tmp_img.texture.id = m_win_data[win_i].win_elems[j].asset_id;

                // Emplace tmp_img to textures vector
                LOG("Emplacing textures");
                m_p_tex->emplace (
                    m_p_tex->begin() + m_win_data[win_i].tex_bounds.second,
                    tmp_img
                );
                m_win_data[win_i].tex_bounds.second++;

                tmp_assets[i].tex_id = m_win_data[win_i].win_elems[j].asset_id;
                tmp_assets[i].asset_mode = DENG_ASSET_MODE_2D_TEXTURE_MAPPED;
                tmp_assets[i].vertices.size = m_win_data[win_i].win_elems[j].mapped_vert.size();
                tmp_assets[i].vertices.p_tex_mapped_vert_data_2d = m_win_data[win_i].win_elems[j].mapped_vert.data();
                break;
            }

            case ELEMENT_COLOR_MODE_UNMAPPED:
                tmp_assets[i].asset_mode = DENG_ASSET_MODE_2D_UNMAPPED;
                tmp_assets[i].vertices.size = m_win_data[win_i].win_elems[j].unmapped_vert.size();
                tmp_assets[i].vertices.p_unmapped_vert_data_2d = m_win_data[win_i].win_elems[j].unmapped_vert.data();
                break;
            
            default:
                break;
            }
        }

        // Insert created assets to asset array
        LOG (
            "Asset bounds: " + 
            std::to_string(m_win_data[win_i].asset_bounds.first) + 
            ", " +
            std::to_string(m_win_data[win_i].asset_bounds.second)
        );
        LOG("Asset_c: " + std::to_string(m_p_assets->size()));
        m_p_assets->insert (
            m_p_assets->begin() + m_win_data[win_i].asset_bounds.second,
            tmp_assets.begin(),
            tmp_assets.end()
        );
        LOG("Asset_c after: " + std::to_string(m_p_assets->size()));
        m_win_data[win_i].asset_bounds.second += (deng_ui32_t) tmp_assets.size();

        // Check if buffers need to be reallocated 
        if(update_buffers) {
            m_p_vk_frame_upd->lockFrame();
            m_p_vk_frame_upd->reallocBuffer();
            m_p_vk_frame_upd->updateTextures(m_win_data[win_i].tex_bounds);
            m_p_vk_frame_upd->updateCmdBuffers();
            m_p_vk_frame_upd->unlockFrame();
        }
        m_p_res_mut->unlock();
    }
    

    /* Push new cursor drop down menu instance */
    void __EH::pushCursorDDM (
        const std::string &window_id,
        const std::vector<DDMElement> &elems,
        deng_ObjVertData2D *active_verts,
        deng_ui32_t active_vert_c,
        deng_bool_t update_buffers
    ) {
        if(!elems.size()) RUN_ERR("Cannot push zero elements");
        m_p_res_mut->lock();

        size_t ddm_index = __findCursorDDMIndex(window_id);
        m_cur_ddm_infos[ddm_index].active_verts = active_verts;
        m_cur_ddm_infos[ddm_index].active_vert_c = active_vert_c;
        m_cur_ddm_infos[ddm_index].elems = elems;

        // Check if element parent_id and ddm_id match
        if(elems[0].parent_id != m_cur_ddm_infos[ddm_index].ddm_id) {
            for(size_t i = 0; i < m_cur_ddm_infos[ddm_index].elems.size(); i++)
                m_cur_ddm_infos[ddm_index].elems[i].parent_id = m_cur_ddm_infos[ddm_index].ddm_id;
        }

        m_cur_ddm_infos[ddm_index].asset_bounds.first = m_p_assets->size();
        m_cur_ddm_infos[ddm_index].tex_bounds.first = m_p_tex->size();
        m_cur_ddm_infos[ddm_index].tex_bounds.second = m_p_tex->size();
        m_cur_ddm_infos[ddm_index].ddm_size.first = 0;
        m_cur_ddm_infos[ddm_index].ddm_size.second = 0;
        m_p_assets->resize(m_p_assets->size() + elems.size());
        
        deng_bool_t is_width_setup = false;
        for(size_t i = m_cur_ddm_infos[ddm_index].asset_bounds.first, j = 0; i < m_p_assets->size(); i++, j++) {
            m_cur_ddm_infos[ddm_index].elems[j].asset_id = (char*) calloc (
                m_cur_ddm_infos[ddm_index].elems[j].child_id.size() + m_cur_ddm_infos[ddm_index].elems[j].parent_id.size() + 2,
                sizeof(char)
            );

            if(m_cur_ddm_infos[ddm_index].elems[j].child_id != "") {
                sprintf ( 
                    m_cur_ddm_infos[ddm_index].elems[j].asset_id,
                    "%s#%s",
                    m_cur_ddm_infos[ddm_index].elems[j].parent_id.c_str(),
                    m_cur_ddm_infos[ddm_index].elems[j].child_id.c_str()
                );
            }

            else {
                strcpy (
                    m_cur_ddm_infos[ddm_index].elems[j].asset_id, 
                    m_cur_ddm_infos[ddm_index].elems[j].parent_id.c_str()
                );
            }

            // Base asset information
            m_p_assets->at(i).id = m_cur_ddm_infos[ddm_index].elems[j].asset_id;
            m_p_assets->at(i).is_shown = false;
            m_p_assets->at(i).indices.size = m_cur_ddm_infos[ddm_index].elems[j].indices.size();
            m_p_assets->at(i).indices.p_indices = m_cur_ddm_infos[ddm_index].elems[j].indices.data();

            // Push vertices to assets vector
            if(m_cur_ddm_infos[ddm_index].elems[j].color_mode == ELEMENT_COLOR_MODE_UNMAPPED) {
                m_p_assets->at(i).asset_mode = DENG_ASSET_MODE_2D_UNMAPPED;
                m_p_assets->at(i).vertices.size = m_cur_ddm_infos[ddm_index].elems[j].unmapped_vert.size();
                m_p_assets->at(i).vertices.p_unmapped_vert_data_2d = m_cur_ddm_infos[ddm_index].elems[j].unmapped_vert.data();

                if(!is_width_setup) {
                    m_cur_ddm_infos[ddm_index].ddm_size.first = m_cur_ddm_infos[ddm_index].elems[j].unmapped_vert[1].vert_data.vert_x - 
                    m_cur_ddm_infos[ddm_index].elems[j].unmapped_vert[0].vert_data.vert_x;
                    is_width_setup = true;
                }
                m_cur_ddm_infos[ddm_index].ddm_size.second += m_cur_ddm_infos[ddm_index].elems[j].unmapped_vert[3].vert_data.vert_y - 
                m_cur_ddm_infos[ddm_index].elems[j].unmapped_vert[0].vert_data.vert_y;
            }

            else if(m_cur_ddm_infos[ddm_index].elems[j].color_mode == ELEMENT_COLOR_MODE_TEXTURE_MAPPED) {
                deng::vulkan::TextureImage tmp_img;
                tmp_img.texture.id = m_p_assets->at(i).id;
                tmp_img.texture.description = NULL;
                tmp_img.texture.pixel_data.width = m_cur_ddm_infos[ddm_index].elems[j].tex_box.first;
                tmp_img.texture.pixel_data.height = m_cur_ddm_infos[ddm_index].elems[j].tex_box.second;
                tmp_img.texture.pixel_data.size = m_cur_ddm_infos[ddm_index].elems[j].texture.size();
                tmp_img.texture.pixel_data.p_pixel_data = m_cur_ddm_infos[ddm_index].elems[j].texture.data();

                m_p_tex->emplace (
                    m_p_tex->begin() + m_cur_ddm_infos[ddm_index].tex_bounds.second,
                    tmp_img
                );

                m_cur_ddm_infos[ddm_index].tex_bounds.second = m_p_tex->size();

                m_p_assets->at(i).asset_mode = DENG_ASSET_MODE_2D_TEXTURE_MAPPED;
                m_p_assets->at(i).vertices.size = m_cur_ddm_infos[ddm_index].elems[j].mapped_vert.size();
                m_p_assets->at(i).vertices.p_tex_mapped_vert_data_2d = m_cur_ddm_infos[ddm_index].elems[j].mapped_vert.data();
                m_p_assets->at(i).tex_id = m_cur_ddm_infos[ddm_index].elems[j].asset_id;
            }
        }

        m_cur_ddm_infos[ddm_index].asset_bounds.second = (*m_p_assets).size();
        m_p_res_mut->unlock();
    }


    /* Toggle ddm visibility by ddm id */
    void __EH::toggleDDM(const std::string &ddm_id) {
        size_t i;
        deng_bool_t is_ddm = false;
        for(i = 0; i < m_cur_ddm_infos.size(); i++) {
            if(m_cur_ddm_infos[i].ddm_id == ddm_id) {
                is_ddm = true;
                break;
            }
        }

        if(!is_ddm) return;
        toggleVisibility (
            m_cur_ddm_infos[i].asset_bounds,
            &m_cur_ddm_infos[i].is_visible
        );
    }


    /* Toggle window visibility by window id */
    void __EH::toggleWindow(const std::string &window_id) {
        size_t i;
        deng_bool_t is_win = false;
        for(i = 0; i < m_win_data.size(); i++) {
            if(m_win_data[i].window_id == window_id) {
                is_win = true;
                break;
            }
        }

        if(!is_win) return;
        toggleVisibility (
            m_win_data[i].asset_bounds,
            &m_win_data[i].is_visible
        );
    }


    /* 
     * Toggle asset visibility by bounds 
     * This method automatically updates commandbuffers
     */
    void __EH::toggleVisibility (
        dengMath::vec2<deng_ui32_t> bounds, 
        deng_bool_t *p_vis_flag
    ) {
        *p_vis_flag = !(*p_vis_flag);
        invertVisibilityByBounds(bounds);

        m_p_vk_frame_upd->lockFrame();
        m_p_vk_frame_upd->updateCmdBuffers();
        m_p_vk_frame_upd->unlockFrame();
    }


    /* Check if window has any elements and if needed erase the window */
    void __EH::checkWindowElements() {
        for(deng_i32_t i = 0; i < (deng_i32_t) m_win_data.size(); i++) {
            if(!m_win_data[i].win_elems.size()) {
                LOG("Erasing window: " + std::string(m_win_data[i].window_id));
                m_win_data.erase(m_win_data.begin() + i);
                i--;
                if(i < 0) i = 0;
            }
        }
    }


    /* Remove removed elements from asset vector */
    void __EH::windowAssetRmSync(const std::string &win_id) {
        *m_fi.p_float_flag = false;
        size_t i;

        // Find appropriate window data
        deng_bool_t found_win = false;
        size_t win_index = 0;
        for(i = 0; i < m_win_data.size(); i++) {
            if(m_win_data[i].window_id == win_id) {
                found_win = true;
                win_index = i;
            }

            // Decrement next window element bounds
            else if(found_win) {
                m_win_data[i].asset_bounds.first -= m_win_data[win_index].asset_bounds.second - m_win_data[win_index].asset_bounds.first;
                m_win_data[i].asset_bounds.second -= m_win_data[win_index].asset_bounds.second - m_win_data[win_index].asset_bounds.first;

                m_win_data[i].tex_bounds.first -= m_win_data[win_index].tex_bounds.second - m_win_data[win_index].tex_bounds.first;
                m_win_data[i].tex_bounds.second -= m_win_data[win_index].tex_bounds.second - m_win_data[win_index].tex_bounds.first;
            }
        }

        // Check if window element was found
        if(!found_win)
            RUN_ERR("Invalid window id");

        // Decrement cursor drop down menu bounds
        for(i = 0; i < m_cur_ddm_infos.size(); i++) {
            if(m_cur_ddm_infos[i].asset_bounds.first >= m_win_data[win_index].asset_bounds.second) {
                m_cur_ddm_infos[i].asset_bounds.first -= m_win_data[win_index].asset_bounds.second - m_win_data[win_index].asset_bounds.first;
                m_cur_ddm_infos[i].asset_bounds.second -= m_win_data[win_index].asset_bounds.second - m_win_data[win_index].asset_bounds.first;
            }
        }

        // Erase window assets
        if(m_win_data[win_index].asset_bounds.second - m_win_data[win_index].asset_bounds.first > 0) {
            m_p_assets->erase (
                m_p_assets->begin() + m_win_data[win_index].asset_bounds.first,
                m_p_assets->begin() + m_win_data[win_index].asset_bounds.second
            );
        }

        // Erase window texture data
        if(m_win_data[win_index].tex_bounds.second - m_win_data[win_index].tex_bounds.first > 0) {
            // Free all vulkan resources
            for(size_t i = m_win_data[win_index].tex_bounds.first; i < m_win_data[win_index].tex_bounds.second; i++) {
                vkFreeDescriptorSets (
                    m_p_ic->getDev(),
                    m_p_desc_c->getTexMappedDP(),
                    m_p_tex->at(i).descriptor_sets.size(),
                    m_p_tex->at(i).descriptor_sets.data()
                );

                vkDestroySampler (
                    m_p_ic->getDev(),
                    m_p_tex->at(i).sampler,
                    NULL
                );

                vkDestroyImageView (
                    m_p_ic->getDev(),
                    m_p_tex->at(i).image_view,
                    NULL
                );

                vkDestroyImage (
                    m_p_ic->getDev(),
                    m_p_tex->at(i).image,
                    NULL
                );

                vkFreeMemory (
                    m_p_ic->getDev(),
                    m_p_tex->at(i).image_mem,
                    NULL
                );
            }
            m_p_tex->erase (
                m_p_tex->begin() + m_win_data[win_index].tex_bounds.first,
                m_p_tex->begin() + m_win_data[win_index].tex_bounds.second
            );
        }
        // Erase elements
        m_win_data[win_index].win_elems.clear();
    }


    /* Invert all assets' visibility that are between bounds */
    void __EH::invertVisibilityByBounds(dengMath::vec2<deng_ui32_t> bounds) {
        for(size_t i = bounds.first; i < bounds.second; i++)
            (*m_p_assets)[i].is_shown = !(*m_p_assets)[i].is_shown;
    }


    /***********************************/
    /********** EVENTS THREAD **********/
    /***********************************/

    /* Events constructor */
    Events::Events(EventInfo &ev_info) :
    __EH (
        ev_info.p_assets,
        ev_info.p_vk_textures,
        &frame_upd,
        this,
        &m_float_mode,
        ev_info.p_ic,
        ev_info.p_desc_c,
        ev_info.p_res_mut
    ), frame_upd(&m_info) { 
        m_info = ev_info;
        std::thread input_thread(&Events::__inputPoll, this);
        input_thread.detach();
    }


    /* Wait until no btn input is found */
    void Events::__waitForNoInput(deng_MouseButton btn) {
        deng_bool_t is_btn = true;

        ext_mii.mut.unlock();
        m_info.p_res_mut->unlock();
        
        while(is_btn) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DENGUI_EV_INPUT_SLEEP_INTERVAL));
            
            // Check if btn is found
            is_btn = false;
            ext_mii.mut.lock();
            is_btn = deng_FindKeyStatus (
                DENG_KEY_UNKNOWN, 
                btn, 
                DENG_INPUT_TYPE_MOUSE, 
                DENG_INPUT_EVENT_TYPE_ACTIVE
            );
            ext_mii.mut.unlock();
        }

        ext_mii.mut.lock();
        m_info.p_res_mut->lock();
    }


    /* Main input polling method (seperate thread) */
    void Events::__inputPoll() {
        dengMath::vec2<deng_vec_t> vec_mouse;
        deng_MouseButton wait_btn = DENG_MOUSE_BTN_UNKNOWN;

        m_info.p_run_mut->lock();
        while(deng_IsRunning()) {
            m_info.p_res_mut->lock();
            ext_mii.mut.lock();
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

            // Poll events if mouse input is available for it
            if (ext_mii.is_mouse_input) {
                // Check if release is needed
                if (m_wait_release && !m_float_mode) {
                    __waitForNoInput(wait_btn);
                    m_wait_release = false;
                }

                else if (!m_is_ddm && !m_float_mode) {
                    // Check if drop down menu needs to be spawn
                    m_is_ddm = __EH_CheckDDMSpawn(vec_mouse);
                    m_wait_release = m_is_ddm;
                    if (m_is_ddm) {
                        wait_btn = DENGUI_DEFAULT_DROP_DOWN_MENU_TRIGGER;
                        ext_mii.mut.unlock();
                        m_info.p_res_mut->unlock();
                        continue;
                    }
                }

                else if (m_is_ddm && __EH_GetDDMInfos()->size() && !m_float_mode) {
                    // Check if drop down menu must be despawn
                    m_wait_release = __EH_UpdateDDMEv(vec_mouse, this, &m_is_ddm);
                    if (m_wait_release) {
                        wait_btn = DENGUI_DEFAULT_DROP_DOWN_MENU_TRIGGER;
                        ext_mii.mut.unlock();
                        m_info.p_res_mut->unlock();
                        continue;
                    }
                }

                if (!m_float_mode) {
                    if (__EH_UpdateWindows(vec_mouse, this, wait_btn))
                        m_wait_release = true;
                }

                else __EH_UpdateFloating(vec_mouse);
            }
            
            ext_mii.mut.unlock();
            m_info.p_res_mut->unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(DENGUI_ITERATION_SLEEP_INTERVAL));
        }

        LOG("Exiting input thread");
        m_info.p_run_mut->unlock();
    }


    /* 
     * Get all window elements from event class                 
     * These methods are made to be used from the main thread   
     */
    dengMath::vec2<deng_ui32_t> Events::getWindowAssetBounds(const std::string &win_id) {
        WindowData *p_win = __EH_FindWindow(win_id);
        if(p_win)
            return p_win->asset_bounds;
        else
            return {0, 0};
    }

    dengMath::vec2<deng_ui32_t> Events::getWindowTexBounds(const std::string &win_id) {
        WindowData *p_win = __EH_FindWindow(win_id);
        if(p_win)
            return p_win->tex_bounds;
        else 
            return {0, 0};
    }

    std::vector<WindowElement> *Events::getWinElems(const std::string &win_id) {
        WindowData *p_win = __EH_FindWindow(win_id);
        if(p_win)
            return &p_win->win_elems;
        else 
            return NULL;
    }

    std::vector<deng_Asset> *Events::getAssets() {
        return m_info.p_assets;
    }
}
