#include <string_view>
#include <GLFW/glfw3.h>
#include <utility>

namespace siege_of_nodes {

class Window{
    public:
        Window(int width, int height, std::string_view title);
        
        Window(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        Window(Window&&) noexcept;
        Window& operator=(Window) noexcept;

        ~Window() noexcept;

        friend void swap(Window& a, Window& b) noexcept;

        bool shouldClose() const;
        void swap_buffers() const;
        void poll() const;

    private:
        GLFWwindow* window = nullptr;
};

inline void swap(Window& a, Window& b) noexcept {
    std::swap(a.window, b.window);
}

}