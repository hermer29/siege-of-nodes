#include <stdexcept>
#include <glad/glad.h>
#include "Window.hpp"

namespace siege_of_nodes {

Window::Window(int width, int height, std::string_view title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    glfwMakeContextCurrent(window);

    //glfwSwapInterval(0);  // vsync, опционально

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to load OpenGL via GLAD");
    }
}

Window::~Window() noexcept {
    if(window != nullptr) {
        glfwDestroyWindow(window);
    }
}

Window& Window::operator=(Window w) noexcept {
    if(this != &w) {
        swap(*this, w);
    }
    return *this;
}

Window::Window(Window&& w) noexcept {
    swap(*this, w);
}

void Window::poll() const {
    glfwPollEvents();    
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::swap_buffers() const {
    glfwSwapBuffers(window);
}

}