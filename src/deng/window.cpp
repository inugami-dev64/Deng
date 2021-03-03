#define __WINDOW_CPP
#include <deng/window.h>

namespace deng {
    WindowWrap::WindowWrap (
        int x, 
        int y, 
        const char *title
    ) {
        m_size = {static_cast<deng_ui32_t>(x), static_cast<deng_ui32_t>(y)};
        m_title = (char*) title;
        m_pixel_size = { 
            2.0 / static_cast<deng_px_t>(m_size.first), 
            2.0 / static_cast<deng_px_t>(m_size.second)};
        m_p_game_window = deng_InitVKSurfaceWindow (
            x, 
            y, 
            m_title, 
            DENG_WINDOW_MODE_FIXED
        );
    }

    WindowWrap::~WindowWrap() {
        deng_DestroyWindow(m_p_game_window);
    }


    /*
     * Either disable or enable virtual mouse mode
     */
    void WindowWrap::setMovement(deng_bool_t is_mov) {    
        if(is_mov) {
            deng_SetMouseCursorMode (
                m_p_game_window, 
                DENG_MOUSE_MODE_VIRTUAL
            );
        } 
        else { 
            deng_SetMouseCursorMode (
                m_p_game_window, 
                DENG_MOUSE_MODE_CURSOR_VISIBLE
            );
        }
        m_is_movement = is_mov;
    }


    dengMath::vec2<deng_px_t> WindowWrap::getPixelSize() {
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

    deng_bool_t WindowWrap::isMovement() {
        return m_is_movement;
    }
}
