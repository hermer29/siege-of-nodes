#include <core/Window.hpp>
#include <core/App.hpp>

using namespace siege_of_nodes;

int main() {
    App app;
    app.init();

    {
        const Window w(800, 600, "Siege of nodes");

        while(true) {  
            w.poll();
    
            if(w.shouldClose())
                break;
    
            w.swap_buffers();
        }
    }
    

    app.terminate();

    return 0;
}