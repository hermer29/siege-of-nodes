#include <GLFW/glfw3.h>
#include "App.hpp"

namespace siege_of_nodes {

void App::init() {
    glfwInit();
}

void App::terminate() {
    glfwTerminate();
}

}