#include "api_core.h"

namespace deng {
    WindowWrap::WindowWrap(const int &x, const int &y, const char *title) {
        this->m_size = {static_cast<uint32_t>(x), static_cast<uint32_t>(y)};
        this->m_title = (char*) title;
        this->m_pixel_size = {2.0 / static_cast<double>(this->m_size.first), 2.0 / static_cast<double>(this->m_size.second)};
        LOG("Pixel size: " + std::to_string(this->m_pixel_size.first) + "/" + std::to_string(this->m_pixel_size.second));
        this->m_p_game_window = init_window(x, y, this->m_title, DENG_WINDOW_MODE_FIXED);
    }

    WindowWrap::~WindowWrap() {
        destroy_window(this->m_p_game_window);
    }

    DENGWindow *WindowWrap::getWindow() {
        return this->m_p_game_window;
    }

    const char *WindowWrap::getTitle() {
        return this->m_title;
    }

    dengMath::vec2<uint32_t> WindowWrap::getSize() {
        return this->m_size;
    }

    deng_InputMode WindowWrap::getInputMode() {
        return this->m_input_mode;
    }

    void WindowWrap::setInputMode(const deng_InputMode &new_input_mode) {    
        switch (new_input_mode)
        {
        case DENG_INPUT_MOVEMENT:
            set_mouse_cursor_mode(this->m_p_game_window, DENG_HIDE_CURSOR);
            break;

        case DENG_INPUT_NONMOVEMENT:
            set_mouse_cursor_mode(this->m_p_game_window, DENG_SHOW_CURSOR);
            break;
        
        default:
            break;
        }

        this->m_input_mode = new_input_mode;
    }

    dengMath::vec2<double> WindowWrap::getPixelSize() {
        return this->m_pixel_size;
    }
}