#include <GLFW/glfw3.h>

#include <glm/trigonometric.hpp>

#include "engine.hpp"
#include "gachiball.hpp"

int main() {
    NGameEngine::TGameEngine engine;
    NGachiBall::TGame game{&engine};

    engine.init();
    engine.run(&game);
    engine.deinit();

    return 0;
}
