#include "api_core.h"

namespace deng {
    WindowWrap::WindowWrap (
        const int &x, 
        const int &y, 
        const char *title
    ) {
        m_size = {static_cast<deng_ui32_t>(x), static_cast<deng_ui32_t>(y)};
        m_title = (char*) title;
        m_pixel_size = {2.0 / static_cast<double>(m_size.first), 2.0 / static_cast<double>(m_size.second)};
        LOG("Pixel size: " + std::to_string(m_pixel_size.first) + "/" + std::to_string(m_pixel_size.second));
        m_p_game_window = deng_InitVKSurfaceWindow(x, y, m_title, DENG_WINDOW_MODE_FIXED);
    }

    WindowWrap::~WindowWrap() {
        deng_DestroyWindow(m_p_game_window);
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

    deng_InputMode WindowWrap::getInputMode() {
        return m_input_mode;
    }

    void WindowWrap::setInputMode(const deng_InputMode &new_input_mode) {    
        switch (new_input_mode)
        {
        case DENG_INPUT_MOVEMENT:
            deng_SetMouseCursorMode (
                m_p_game_window, 
                DENG_MOUSE_MODE_VIRTUAL
            );
            break;

        case DENG_INPUT_NONMOVEMENT:
            deng_SetMouseCursorMode (
                m_p_game_window, 
                DENG_MOUSE_MODE_CURSOR_VISIBLE
            );
            break;
        
        default:
            break;
        }

        m_input_mode = new_input_mode;
    }

    dengMath::vec2<double> WindowWrap::getPixelSize() {
        return m_pixel_size;
    }
}
