#ifndef WINDOWS_H
#define WINDOWS_H

#include "timer.h"

namespace deng
{   
    enum dengInputMode {
        DENG_INPUT_NONMOVEMENT = 0,
        DENG_INPUT_MOVEMENT = 1
    };

    class Window
    {
    private:
        GLFWwindow *m_game_window;
        const char *m_title;
        vec2<uint32_t> m_size;
        dengInputMode m_input_mode;

    public:
        Window(const uint32_t &x, const uint32_t &y, const char *title);
        ~Window();
        GLFWwindow *getWindow();
        const char *getTitle();
        dengInputMode getInputMode();
        vec2<uint32_t> getSize();
        void setInputMode(const dengInputMode &new_input_mode);
    };
}

#endif