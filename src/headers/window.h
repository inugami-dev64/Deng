#ifndef WINDOWS_H
#define WINDOWS_H

#include "vectors.h"

namespace Deng
{   
    class Window
    {
        private:
            GLFWwindow *gameWindow;
            const char *title;
            Vector2D size;

        public:
            Window(const uint32_t &x, const uint32_t &y, const char *title);
            ~Window();
            GLFWwindow *getWindow();
            const char *getTitle();
            Vector2D getSize();
    };
}

#endif