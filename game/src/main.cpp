#include <GLFW/glfw3.h>

#include <glm/trigonometric.hpp>

#include "engine.hpp"

int main() {
    NGameEngine::TGameEngine engine;
    engine.init();

    std::unique_ptr<NCamera::ICamera> camera = NCamera::CreateRotatingCamera(
        {0.f, 0.f, 0.f}, glm::radians(30.f), 20.f
    );
    engine.bindCamera(camera.get());

    engine.run();
    engine.deinit();

    return 0;
}
