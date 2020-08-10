#include "headers/renderer.h"

namespace Deng {
    Window::Window(const uint32_t &x, const uint32_t &y, const char *title) {
        this->size.x = x;
        this->size.y = y;

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

    Vector2D Window::getSize() {
        return this->size;
    }
}