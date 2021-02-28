#include "../../headers/deng/api_core.h" 
#include <string>

extern dengui::MouseInputInfo ext_mii;
namespace dengui {
    
    /***************************************/
    /********* FRAME UPDATER CLASS *********/
    /***************************************/
    __FrameUpdater::__FrameUpdater(EventInfo *p_info) 
    { m_p_info = p_info; }


    /* External asset mutex lockers and unlockers */
    void __FrameUpdater::lockAssets() {
        m_p_info->p_res_mut->lock();
    }


    void __FrameUpdater::unlockAssets() {
        m_p_info->p_res_mut->unlock();
    }

    
    /* External frame mutex lockers and unlockers */
    void __FrameUpdater::lockFrame() {
        m_p_info->p_frame_mut->lock();
        vkDeviceWaitIdle(m_p_info->p_ic->getDev());
        vkQueueWaitIdle(m_p_info->p_ic->getQFF().graphics_queue);
        vkQueueWaitIdle(m_p_info->p_ic->getQFF().present_queue);
    }

    void __FrameUpdater::unlockFrame() {
        m_p_info->p_frame_mut->unlock();
    }


    /* Reallocate vertex buffer */
    void __FrameUpdater::reallocBuffer() {
        vkDestroyBuffer (
            m_p_info->p_ic->getDev(),
            m_p_info->p_ra->getBD()->main_buffer,
            NULL
        );

        vkFreeMemory (
            m_p_info->p_ic->getDev(),
            m_p_info->p_ra->getBD()->main_buffer_memory,
            NULL
        );

        m_p_info->p_ra->mkBuffers (
            m_p_info->p_ic->getDev(),
            m_p_info->p_ic->getGpu(),
            m_p_info->p_dc->getComPool(),
            m_p_info->p_ic->getQFF().graphics_queue
        );
    }

    
    /* Update vertices for every asset between bounds */
    void __FrameUpdater::updateVerts(dengMath::vec2<deng_ui32_t> update_bounds) {
        m_p_info->p_ra->remapAssetVerts (
            m_p_info->p_ic->getDev(),
            m_p_info->p_ic->getGpu(),
            m_p_info->p_dc->getComPool(),
            m_p_info->p_ic->getQFF().graphics_queue,
            update_bounds
        );
    }


    /* Update descriptor sets with texture sampling */
    void __FrameUpdater::updateTexDS (
        deng_bool_t realloc_ds, 
        dengMath::vec2<deng_ui32_t> *p_tex_bounds
    ) {
        m_p_info->p_desc_c->updateTexDescriptors (
            m_p_info->p_ic->getDev(),
            m_p_info->sc_img_size,
            realloc_ds,
            p_tex_bounds,
            m_p_info->p_ra->getBD()
        );
    }


    /* Update newly pushed texture objects */
    void __FrameUpdater::updateTextures(dengMath::vec2<deng_ui32_t> tex_bounds) {
        m_p_info->p_ra->mkTextureImages (
            m_p_info->p_ic->getDev(),
            m_p_info->p_ic->getGpu(),
            m_p_info->p_dc->getComPool(),
            m_p_info->p_ic->getLFSupport(),
            tex_bounds,
            m_p_info->p_ic->getQFF().graphics_queue,
            m_p_info->sc_img_size
        );

        updateTexDS (
            true, 
            &tex_bounds
        );
    }


    /* Update command buffers */
    void __FrameUpdater::updateCmdBuffers() {
        m_p_info->p_dc->recordMainCmdBuffers (
            m_p_info->renderpass,
            m_p_info->ext,
            m_p_info->background,
            *m_p_info->p_ra->getBD()
        );
    }


    /*************************************/
    /******* ELEMENT HANDLER CLASS *******/
    /*************************************/
    __EH::__EH (
        std::vector<deng_Asset> *p_assets, 
        std::vector<deng::TextureImageData> *p_tex,
        __FrameUpdater *p_frame_upd,
        Events *p_ev,
        deng::InstanceCreator *p_ic,
        deng::DescriptorCreator *p_desc_c,
        std::mutex *p_res_mut
    ) {
        m_p_frame_upd = p_frame_upd;
        m_p_assets = p_assets;
        m_p_tex = p_tex;
        m_p_res_mut = p_res_mut;
        m_p_ic = p_ic;
        m_p_desc_c = p_desc_c;
        m_cur_ddm_transform.row1 = {1.0f, 0.0f, 0.0f};
        m_cur_ddm_transform.row2 = {0.0f, 1.0f, 0.0f};
        m_cur_ddm_transform.row3 = {0.0f, 0.0f, 1.0f};
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
     * Check if drop down menu should be spawn
     */
    deng_bool_t __EH::__EH_CheckDDMSpawn(dengMath::vec2<deng_vec_t> mouse_pos) {
        size_t i;
        deng_bool_t spawn_menu = false;
        ext_mii.mut.lock();

        m_cur_ddm_transform.row1.third = mouse_pos.first;
        m_cur_ddm_transform.row2.third = mouse_pos.second;

        // Search for appropriate button stroke 
        for(i = 0; i < (size_t) ext_mii.active_btn_c; i++) {
            if(ext_mii.active_btn[i] == DENGUI_DEFAULT_DROP_DOWN_MENU_TRIGGER) {
                spawn_menu = true;
                break;
            }
        }

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
                    if(m_cur_ddm_transform.row1.third > (1.0f - m_cur_ddm_infos[i].ddm_size.first))
                        m_cur_ddm_transform.row1.third = 1.0f - m_cur_ddm_infos[i].ddm_size.first;
                    if(m_cur_ddm_transform.row2.third > 1.0f - (m_cur_ddm_infos[i].ddm_size.second))
                        m_cur_ddm_transform.row2.third = 1.0f - m_cur_ddm_infos[i].ddm_size.second;

                    LOG (
                        "Spawn bounds: " + 
                        std::to_string(m_cur_ddm_infos[i].asset_bounds.first) +
                        ", " +
                        std::to_string(m_cur_ddm_infos[i].asset_bounds.second)
                    );
                    dengMath::Transformer::apply2DModelMatrix (
                        m_cur_ddm_infos[i].asset_bounds,
                        m_p_assets,
                        m_cur_ddm_transform
                    );
                    
                    m_p_frame_upd->lockFrame();
                    m_p_frame_upd->updateVerts(m_cur_ddm_infos[i].asset_bounds);
                    m_p_frame_upd->unlockFrame();
                    toggleVisibility (
                        m_cur_ddm_infos[i].asset_bounds,
                        &m_cur_ddm_infos[i].is_visible
                    );
                    LOG("Menu spawned");
                }
            }
        }
        
        ext_mii.mut.unlock();
        return spawn_menu;
    }


    /* 
     * Check for any callback events on drop down menus
     */
    deng_bool_t __EH::__EH_UpdateDDMEv (
        dengMath::vec2<deng_vec_t> mouse_pos, 
        Events *p_ev,
        deng_bool_t *p_ddm_flag
    ) {
        size_t i, j;
        deng_i32_t key_index;
        std::array<deng_ObjVertData2D, 4> verts;

        ext_mii.mut.lock();

        // Check each drop menu instance for closing if needed 
        DDMCallback callback = NULL;
        if(ext_mii.active_btn_c ) {
            for(i = 0; i < m_cur_ddm_infos.size(); i++) {
                callback = NULL;
                if(!m_cur_ddm_infos[i].is_visible)
                    continue;

                // Check for collision with each element 
                for(j = m_cur_ddm_infos[i].asset_bounds.first; j < m_cur_ddm_infos[i].asset_bounds.second; j++) {
                    if (
                        findPtCollision (
                            mouse_pos, 
                            &m_p_assets->at(j),
                            {0, 4}
                        )
                    ) {
                        for(key_index = 0; key_index < ext_mii.active_btn_c; key_index++) {
                            size_t elem_i = j - m_cur_ddm_infos[i].asset_bounds.first;

                            callback = NULL;
                            switch(ext_mii.active_btn[key_index])
                            {
                            case DENG_MOUSE_BTN_1:
                                if(m_cur_ddm_infos[i].elems[elem_i].onLMBClickFunc)
                                    callback = m_cur_ddm_infos[i].elems[elem_i].onLMBClickFunc;
                                break;

                            case DENG_MOUSE_BTN_2:
                                if(m_cur_ddm_infos[i].elems[elem_i].onMMBClickFunc)
                                    callback = m_cur_ddm_infos[i].elems[elem_i].onMMBClickFunc;
                                break;

                            case DENG_MOUSE_BTN_3:
                                if(m_cur_ddm_infos[i].elems[elem_i].onRMBClickFunc)
                                    callback = m_cur_ddm_infos[i].elems[elem_i].onRMBClickFunc;
                                break;

                            default:
                                break;
                            }

                            if(callback) {
                                m_p_res_mut->unlock();
                                callback (
                                    &m_cur_ddm_infos[i].elems[elem_i],
                                    p_ev
                                );
                                *p_ddm_flag = false;
                                m_p_res_mut->lock();
                                dengMath::mat3<deng_vec_t> inv_mat = m_cur_ddm_transform.inv();
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
                    }
                }

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
                    LOG("Closing");
                    dengMath::mat3<deng_vec_t> inv_mat = m_cur_ddm_transform.inv();
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

        ext_mii.mut.unlock();
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
        size_t win_i, i, j;
        // Check if collision with any window element has happened and if click callback should be triggered
        for(win_i = 0; win_i < m_win_data.size(); win_i++) {
            ext_mii.mut.lock();
            for(i = 0; i < m_win_data[win_i].win_elems.size(); i++) {
                if(!m_win_data[win_i].win_elems[i].is_interactive)
                    continue;

                // Generic element collision detection
                if
                (
                    m_p_assets->at(m_win_data[win_i].asset_bounds.first + i).is_shown &&
                    m_win_data[win_i].win_elems[i].is_interactive && 
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
                    for(j = 0; j < (size_t) ext_mii.active_btn_c; j++) {
                        callback = NULL;
                        feedback = ext_mii.active_btn[j];
                        switch (ext_mii.active_btn[j])
                        {
                        case DENG_MOUSE_BTN_1:
                            callback = m_win_data[win_i].win_elems[i].onLMBClickFunc;
                            break;

                        case DENG_MOUSE_BTN_2:
                            callback = m_win_data[win_i].win_elems[i].onMMBClickFunc;
                            break;

                        case DENG_MOUSE_BTN_3:
                            callback = m_win_data[win_i].win_elems[i].onRMBClickFunc;
                            break;

                        case DENG_MOUSE_SCROLL_DOWN:
                            callback = m_win_data[win_i].win_elems[i].onScrDownFunc;
                            break;

                        case DENG_MOUSE_SCROLL_UP:
                            callback = m_win_data[win_i].win_elems[i].onScrUpFunc;
                            break;
                        
                        default:
                            break;
                        }

                        if(callback) {
                            m_p_res_mut->unlock();
                            callback (
                                &m_win_data[win_i].win_elems[i],
                                p_ev
                            );

                            ext_mii.mut.unlock();
                            m_p_res_mut->lock();
                            return true;
                        }
                    }
                }
            }
            ext_mii.mut.unlock();
        }

        return false;
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
                // Add new TextureImageData instance to its vector
                deng::TextureImageData tmp_img;
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
        m_win_data[win_i].asset_bounds.second += tmp_assets.size();

        // Check if buffers need to be reallocated 
        if(update_buffers) {
            m_p_frame_upd->lockFrame();
            m_p_frame_upd->reallocBuffer();
            m_p_frame_upd->updateTextures(m_win_data[win_i].tex_bounds);
            m_p_frame_upd->updateCmdBuffers();
            m_p_frame_upd->unlockFrame();
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
                deng::TextureImageData tmp_img;
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

        m_p_frame_upd->lockFrame();
        m_p_frame_upd->updateCmdBuffers();
        m_p_frame_upd->unlockFrame();
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
        ev_info.p_textures,
        &frame_upd,
        this,
        ev_info.p_ic,
        ev_info.p_desc_c,
        ev_info.p_res_mut
    ), frame_upd(&m_info) { 
        m_info = ev_info;
        std::thread input_thread(&Events::__inputPoll, this);
        input_thread.detach();
    }

    Events::~Events() {
        delete m_info.p_dc;
        delete m_info.p_ra;
    }


    /* Wait until no btn input is found */
    void Events::__waitForNoInput(deng_MouseButton btn) {
        deng_bool_t is_btn = true;
        deng_i32_t index;

        m_info.p_res_mut->unlock();
        ext_mii.mut.unlock();
        
        while(is_btn) {
            LOG("Waiting...");
            std::this_thread::sleep_for(std::chrono::milliseconds(DENGUI_EV_INPUT_SLEEP_INTERVAL));
            LOG("Wait done");
            
            // Check if btn is found
            is_btn = false;
            LOG("Lock test");
            ext_mii.mut.lock();
            LOG("Lock test1");
            for(index = 0; index < ext_mii.active_btn_c; index++) {
                if(ext_mii.active_btn[index] == btn) {
                    is_btn = true;
                    break;
                }
            }
            ext_mii.mut.unlock();
        }

        m_info.p_res_mut->lock();
    }


    /* Main input polling method (seperate thread) */
    void Events::__inputPoll() {
        dengMath::vec2<deng_vec_t> vec_mouse;
        deng_MouseButton wait_btn = DENG_MOUSE_BTN_UNKNOWN;

        while(deng_IsRunning(m_p_win)) {
            m_info.p_res_mut->lock();
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
                
            // Check if release is needed
            if(m_wait_release) {
                __waitForNoInput(wait_btn);
                m_wait_release = false;
            }

            else if(!m_is_ddm) {
                // Check if drop down menu needs to be spawn
                m_is_ddm = __EH_CheckDDMSpawn(vec_mouse);
                m_wait_release = m_is_ddm;
                if(m_is_ddm) {
                    wait_btn = DENGUI_DEFAULT_DROP_DOWN_MENU_TRIGGER;
                    m_info.p_res_mut->unlock();
                    continue;
                }    
            }

            else if(m_is_ddm && __EH_GetDDMInfos()->size()) {
                // Check if drop down menu must be despawn
                m_wait_release = __EH_UpdateDDMEv(vec_mouse, this, &m_is_ddm);
                if(m_wait_release) {
                    wait_btn = DENGUI_DEFAULT_DROP_DOWN_MENU_TRIGGER;
                    m_info.p_res_mut->unlock();
                    continue;
                }
            }

            if(__EH_UpdateWindows(vec_mouse, this, wait_btn))
                m_wait_release = true;
            
            m_info.p_res_mut->unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(DENGUI_ITERATION_SLEEP_INTERVAL));
        }
    }


    /* Get all window elements from event class                 *
     * These methods are made to be used from the main thread   */
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
