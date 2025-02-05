#include <GLFW/glfw3.h>

#include "engine.hpp"

int main() {
    NGameEngine::TGameEngine engine;

    engine.init();
    engine.run();
    engine.deinit();

    return 0;
}
