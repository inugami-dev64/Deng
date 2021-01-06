#include "../../core/api_core.h"

extern dengui::MouseInputInfo mii;

namespace dengui {
    /***********************************/
    /********** EVENTS THREAD **********/
    /***********************************/

    /* Events constructor */
    Events::Events (
        std::vector<ElementInfo> &elems, 
        dengMath::vec2<uint32_t> deng_window_area
    ) {
        m_elem_infos = elems;
        m_deng_window_size = deng_window_area;
        std::thread input_thread(&Events::inputPoll, this);
        input_thread.detach();
    }


    /* Find triangle based collision */
    bool Events::findCollision(dengMath::vec2<float> point, ElementInfo *p_elem) {
        size_t l_index, r_index;
        bool is_colliding = false;
        dengMath::vec2<float> vc;
        dengMath::vec2<float> vn;

        // Check every triangle for potential collision
        switch (p_elem->color_mode)
        {
        case ELEMENT_COLOR_MODE_UNMAPPED:
            // printf("MOUSE: %f, %f\n", point.first, point.second);
            for(l_index = 0; l_index < p_elem->unmapped_vert.size(); l_index++) {
                r_index = l_index + 1;
                if(r_index == p_elem->unmapped_vert.size()) r_index = 0;

                vc = *(dengMath::vec2<float>*) &p_elem->unmapped_vert[l_index].vert_data;
                vn = *(dengMath::vec2<float>*) &p_elem->unmapped_vert[r_index].vert_data;

                if
                (
                    ((vc.second >= point.second && vn.second < point.second) ||
                    (vc.second < point.second && vn.second >= point.second)) &&
                    (point.first < (vn.first - vc.first) * (point.second - vc.second) + vc.first)
                ) is_colliding = !is_colliding;
            }
            break;

        case ELEMENT_COLOR_MODE_TEXTURE_MAPPED:
            for(l_index = 0; l_index < p_elem->mapped_vert.size(); l_index++) {
                r_index = l_index + 1;
                if(r_index == p_elem->mapped_vert.size()) r_index = 0;

                vc = *(dengMath::vec2<float>*) &p_elem->mapped_vert[l_index].vert_data;
                vn = *(dengMath::vec2<float>*) &p_elem->mapped_vert[r_index].vert_data;

                if
                (
                    ((vc.second >= point.second && vn.second < point.second) ||
                    (vc.second < point.second && vn.second >= point.second)) &&
                    (point.first < (vn.first - vc.first) * (point.second - vc.second) + vc.first)
                ) is_colliding = !is_colliding;
            }
            break;
        }

        return is_colliding;
    }


    /* Wait until no btn input is found */
    void Events::waitForNoInput(DENGMouseButton btn) {
        bool is_btn = true;
        int32_t index;
        
        while(is_btn) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DENGUI_EV_SLEEP_INTERVAL));
            
            // Check if btn is found
            is_btn = false;
            mii.mouse_input_mut.lock();
            for(index = 0; index < mii.active_btn_c; index++) {
                if(mii.active_btn[index] == btn) {
                    is_btn = true;
                    break;
                }
            }
            mii.mouse_input_mut.unlock();
        }
    }

    
    /* Main input polling method */
    void Events::inputPoll() {
        size_t l_index; 
        int32_t r_index;
        dengMath::vec2<float> vec_mouse;

        while(is_running(m_p_win)) {
            // Check if collision with any element has happened and if click callback should be triggered
            for(l_index = 0; l_index < m_elem_infos.size(); l_index++) {
                std::lock_guard<std::mutex> lck(mii.mouse_input_mut);
                vec_mouse.first = dengMath::Conversion::pixelSizeToVector2DSize (
                    (double) mii.mouse_coords.first,
                    m_deng_window_size,
                    DENG_COORD_AXIS_X
                ) - 1.0f;

                vec_mouse.second = dengMath::Conversion::pixelSizeToVector2DSize (
                    (double) mii.mouse_coords.second,
                    m_deng_window_size,
                    DENG_COORD_AXIS_Y
                ) - 1.0f;

                if(findCollision(vec_mouse, &m_elem_infos[l_index])) {
                    // Search for active button clicks
                    for(r_index = 0; r_index < mii.active_btn_c; r_index++) {
                        switch (mii.active_btn[r_index])
                        {
                        case DENG_MOUSE_BTN_1:
                            if(m_elem_infos[l_index].onLMBClickFunc)
                                m_elem_infos[l_index].onLMBClickFunc();
                            break;

                        case DENG_MOUSE_BTN_2:
                            if(m_elem_infos[l_index].onMMBClickFunc)
                                m_elem_infos[l_index].onMMBClickFunc();
                            break;

                        case DENG_MOUSE_BTN_3:
                            if(m_elem_infos[l_index].onRMBClickFunc)
                                m_elem_infos[l_index].onRMBClickFunc();
                            break;

                        case DENG_MOUSE_SCROLL_DOWN:
                            if(m_elem_infos[l_index].onScrDownFunc)
                                m_elem_infos[l_index].onScrDownFunc();
                            break;

                        case DENG_MOUSE_SCROLL_UP:
                            if(m_elem_infos[l_index].onScrUpFunc)
                                m_elem_infos[l_index].onScrUpFunc();
                            break;
                        
                        default:
                            break;
                        }

                        lck.~lock_guard();
                        waitForNoInput(mii.active_btn[r_index]);
                    }
                }
            }

            usleep(DENGUI_EV_REFRESH_INTERVAL);
        }

    }


    /************************************/
    /****** CALLBACK FUNCTIONS **********/
    /************************************/

    void minTriangleCallback() {
        LOG("Minimizing...");
    }

    void closeBtnCallback() {
        LOG("Closing...");
    }
}
