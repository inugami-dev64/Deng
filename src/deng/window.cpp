#define __WINDOW_CPP
#include <deng/window.h>

namespace deng {
    WindowWrap::WindowWrap (
        int x, 
        int y, 
        const char *title
    ) {
        m_title = (char*) title;
        deng_vec_t x_size = (deng_vec_t)m_size.first;
        deng_vec_t y_size = (deng_vec_t)m_size.second;
        m_pixel_size = { 
            2.0f / x_size,
            2.0f / y_size
        };

        m_p_game_window = deng_InitVKSurfaceWindow(
            x,
            y,
            m_title,
            DENG_WINDOW_MODE_FIXED
        );

        m_size = { 
            (deng_ui32_t) m_p_game_window->width, 
            (deng_ui32_t) m_p_game_window->height 
        };
    }

    WindowWrap::~WindowWrap() {
        deng_DestroyWindow(m_p_game_window);
    }


    /*
     * Either disable or enable virtual mouse mode
     */
    void WindowWrap::setVCMode (
        deng_bool_t is_vcp,
        deng_bool_t change_cursor
    ) {    
        if(is_vcp) {
            if (change_cursor) {
                deng_SetMouseCursorMode(
                    m_p_game_window,
                    DENG_MOUSE_MODE_INVISIBLE
                );
            }
            else
                m_p_game_window->vc_data.is_enabled = true;
        } 
        else { 
            if (change_cursor) {
                deng_SetMouseCursorMode(
                    m_p_game_window,
                    DENG_MOUSE_MODE_CURSOR_VISIBLE
                );
            }
            else m_p_game_window->vc_data.is_enabled = false;
        }
        m_is_vc = is_vcp;
    }


    dengMath::vec2<deng_vec_t> WindowWrap::getPixelSize() {
        return m_pixel_size;
    }

    deng_SurfaceWindow *WindowWrap::getWindow() {
        return m_p_game_window;
    }

    const char *WindowWrap::getTitle() {
        return m_title;
    }

    dengMath::vec2<deng_ui32_t> WindowWrap::getSize() {
        return m_size;
    }

    deng_bool_t WindowWrap::isVCP() {
        return m_is_vc;
    }
}
