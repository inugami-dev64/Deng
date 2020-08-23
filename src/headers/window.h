#ifndef WINDOWS_H
#define WINDOWS_H

#include "timer.h"

namespace deng
{   
    enum InputMode {
        DENG_INPUT_NONMOVEMENT = 1,
        DENG_INPUT_MOVEMENT = 2
    };

    class Window
    {
    private:
        GLFWwindow *m_gameWindow;
        const char *m_title;
        vec2<uint32_t> m_size;
        InputMode m_inputMode;

    public:
        Window(const uint32_t &x, const uint32_t &y, const char *title);
        ~Window();
        GLFWwindow *getWindow();
        const char *getTitle();
        InputMode getInputMode();
        vec2<uint32_t> getSize();
        void setInputMode(const InputMode &newInputMode);
    };
}

#endif