#ifndef __PROGRAM_STATUS_HPP__
class SDLWindow;
class Renderer;
class Graphics;
class GraphicsThreadRunner;

namespace GameThingStatus {
    static bool quit = false;
    SDLWindow *window = NULL;
    Renderer *renderer = NULL;
    Graphics *graphics = NULL;
    GraphicsThreadRunner *runner = NULL;

    static void dumpStatus() {
        std::cerr << "Window is at:          " << static_cast<void*>(window) << std::endl;
        std::cerr << "Renderer is at:        " << static_cast<void*>(renderer) << std::endl;
        std::cerr << "Graphics is at:        " << static_cast<void*>(graphics) << std::endl;
        std::cerr << "Graphics Thread is at: " << static_cast<void*>(graphics) << std::endl;
    }
};

#define __PROGRAM_STATUS_HPP__
#endif

