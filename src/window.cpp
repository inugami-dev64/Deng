#include "headers/renderer.h"

namespace deng {
    Window::Window(const uint32_t &x, const uint32_t &y, const char *title) {
        this->m_size = {(float) x, (float) y};

        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        this->m_gameWindow = glfwCreateWindow(this->m_size.x, this->m_size.y, title, nullptr, nullptr);
    }

    Window::~Window() {
        glfwDestroyWindow(this->m_gameWindow);
        glfwTerminate();
    }

    GLFWwindow *Window::getWindow() {
        return this->m_gameWindow;
    }

    const char *Window::getTitle() {
        return this->m_title;
    }

    vec2<uint32_t> Window::getSize() {
        return this->m_size;
    }

    InputMode Window::getInputMode() {
        return this->m_inputMode;
    }

    void Window::setInputMode(const InputMode &newInputMode) {
        switch (newInputMode)
        {
        case DENG_INPUT_MOVEMENT:
            glfwSetInputMode(this->m_gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;

        case DENG_INPUT_NONMOVEMENT:
            glfwSetInputMode(this->m_gameWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        
        default:
            break;
        }

         this->m_inputMode = newInputMode;
    }

}