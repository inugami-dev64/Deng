#include "headers/renderer.h"

namespace Deng {
    Window::Window(const uint32_t &x, const uint32_t &y, const char *title) {
        this->size = {(float) x, (float) y};

        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        this->gameWindow = glfwCreateWindow(this->size.x, this->size.y, title, nullptr, nullptr);
    }

    Window::~Window() {
        glfwDestroyWindow(this->gameWindow);
        glfwTerminate();
    }

    GLFWwindow *Window::getWindow() {
        return this->gameWindow;
    }

    const char *Window::getTitle() {
        return this->title;
    }

    vec2<uint32_t> Window::getSize() {
        return this->size;
    }
}