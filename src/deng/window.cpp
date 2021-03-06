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


#define __WINDOW_CPP
#include <deng/window.h>

namespace deng {
    Window::Window(deng_i32_t x, deng_i32_t y, const char *title) {
        m_title = (char*) title;
        m_p_surface = deng_InitVKSurfaceWindow(x, y, m_title, DENG_WINDOW_MODE_FIXED);

        m_size = dengMath::vec2<deng_ui32_t> {
            static_cast<deng_ui32_t>(m_p_surface->width),
            static_cast<deng_ui32_t>(m_p_surface->height) 
        };

        m_pixel_size = dengMath::vec2<deng_vec_t> {
            static_cast<deng_vec_t>(m_size.first),
            static_cast<deng_vec_t>(m_size.second)
        };
    }

    Window::~Window() {
        deng_DestroyWindow(m_p_surface);
    }


    /// Set virtual cursor mode that locks real cursor to the center of the window
    void Window::toggleVCMode() {    
        if(!m_p_surface->vc_data.is_enabled) {
            deng_SetMouseCursorMode(m_p_surface, DENG_MOUSE_MODE_INVISIBLE);
            m_p_surface->vc_data.is_enabled = true;
        } 

        else { 
            deng_SetMouseCursorMode(m_p_surface, DENG_MOUSE_MODE_CURSOR_VISIBLE);
            m_p_surface->vc_data.is_enabled = false;
        }
        m_is_vc = !m_is_vc;
    }

    
    /// Force set virtual cursor mode
    void Window::changeVCMode(deng_bool_t is_vc) {
        if(m_is_vc == is_vc) return;
        m_is_vc = is_vc;
        if(is_vc) deng_SetMouseCursorMode(m_p_surface, DENG_MOUSE_MODE_INVISIBLE);
        else deng_SetMouseCursorMode(m_p_surface, DENG_MOUSE_MODE_CURSOR_VISIBLE);
    }
    

    /// Hide the cursor's visbility
    void Window::hideCursor() {
        deng_SetMouseCursorMode (
            m_p_surface, 
            DENG_MOUSE_MODE_INVISIBLE
        );
    }


    /// Make the cursor visible
    /// NOTE: There should be a special cursor struct for DENG called deng_Cursor in the future
    /// but for now it is ignored
    void Window::showCursor() {
        deng_SetMouseCursorMode (
            m_p_surface,
            DENG_MOUSE_MODE_CURSOR_VISIBLE
        );
    }


    /// Update window and input devices data
    void Window::update() {
        m_prev_vc_pos.first = m_p_surface->vc_data.x;
        m_prev_vc_pos.second = m_p_surface->vc_data.y;
        deng_UpdateWindow(m_p_surface);
    }


    /// Force specified VCP position to virtual mouse cursor instance
    void Window::forceVCPPos(const dengMath::vec2<deng_px_t> &pos) {
        m_p_surface->vc_data.x = pos.first;
        m_p_surface->vc_data.y = pos.second;
    }


    /// Create new vulkan surface instance
    VkResult Window::initVkSurface (
        VkInstance &instance,
        VkSurfaceKHR &surface
    ) {
        return  deng_InitVKSurface(m_p_surface, &instance, &surface);
    }


    /// Find all required surface extensions
    char **Window::findVulkanSurfaceExtensions (
        deng_ui32_t *p_ext_c, 
        deng_bool_t enable_vl
    ) {
        char **exts;
        deng_GetRequiredVKSurfaceExt (
            m_p_surface,
            &exts,
            p_ext_c,
            enable_vl
        );

        return exts;
    }


    /// Get the current mouse position
    dengMath::vec2<deng_px_t> Window::getMPos() const {
        deng_GetMousePos(m_p_surface, false);

        // Return either virtual cursor position or real cursor position
        if(m_is_vc) {
            return dengMath::vec2<deng_px_t>{
                static_cast<deng_px_t>(m_p_surface->vc_data.x), 
                static_cast<deng_px_t>(m_p_surface->vc_data.y)
            };
        }

        else {
            return dengMath::vec2<deng_px_t>{
                static_cast<deng_px_t>(m_p_surface->mx),
                static_cast<deng_px_t>(m_p_surface->my)
            };
        }
    }


    /// Get the mouse delta compared to previous frame mouse position
    dengMath::vec2<deng_px_t> Window::getMDelta() const {
        if(m_p_surface->vc_data.is_enabled) {
            return dengMath::vec2<deng_px_t>{
                static_cast<deng_px_t>(m_p_surface->vc_data.x - m_prev_vc_pos.first),
                static_cast<deng_px_t>(m_p_surface->vc_data.y - m_prev_vc_pos.second)
            };
        }

        else {
            return dengMath::vec2<deng_px_t>{
                static_cast<deng_px_t>(m_p_surface->mx - m_prev_vc_pos.first),
                static_cast<deng_px_t>(m_p_surface->my - m_prev_vc_pos.second)
            };
        }
    }


    /// Get the vector size for one pixel in surface
    dengMath::vec2<deng_vec_t> Window::getPixelSize() const { return m_pixel_size; }

    /// Get the title of the window
    const char *Window::getTitle() const { return m_title; }

    /// Get the size of the window
    dengMath::vec2<deng_ui32_t> Window::getSize() const { return m_size; }

    /// Check if virtual cursor mode is enabled
    deng_bool_t Window::isVCP() { return m_is_vc; }
}
