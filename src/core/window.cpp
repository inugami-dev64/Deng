#include "deng_core.h"

namespace deng {
    Window::Window(const uint32_t &x, const uint32_t &y, const char *title) {
        this->m_size = {x, y};

        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        this->m_game_window = glfwCreateWindow(this->m_size.first, this->m_size.second, title, nullptr, nullptr);
    }

    Window::~Window() {
        glfwDestroyWindow(this->m_game_window);
        glfwTerminate();
    }

    GLFWwindow *Window::getWindow() {
        return this->m_game_window;
    }

    const char *Window::getTitle() {
        return this->m_title;
    }

    vec2<uint32_t> Window::getSize() {
        return this->m_size;
    }

    dengInputMode Window::getInputMode() {
        return this->m_input_mode;
    }

    void Window::setInputMode(const dengInputMode &new_input_mode) {
        switch (new_input_mode)
        {
        case DENG_INPUT_MOVEMENT:
            glfwSetInputMode(this->m_game_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;

        case DENG_INPUT_NONMOVEMENT:
            glfwSetInputMode(this->m_game_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        
        default:
            break;
        }

        this->m_input_mode = new_input_mode;
    }

}