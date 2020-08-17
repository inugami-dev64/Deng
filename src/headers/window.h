#ifndef WINDOWS_H
#define WINDOWS_H

#include "timer.h"

namespace deng
{   
    class Window
    {
    private:
        GLFWwindow *gameWindow;
        const char *title;
        vec2<uint32_t> size;

    public:
        Window(const uint32_t &x, const uint32_t &y, const char *title);
        ~Window();
        GLFWwindow *getWindow();
        const char *getTitle();
        vec2<uint32_t> getSize();
    };
}

#endif