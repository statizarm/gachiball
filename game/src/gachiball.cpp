#include "gachiball.hpp"

#include <glm/trigonometric.hpp>
#include <thread>

namespace NGachiBall {

TGame::TGame(NGameEngine::TGameEngine* engine) : engine_(engine) {}

void TGame::deinit() { camera_.reset(); }

void TGame::init() {
    assert(!camera_);

    camera_ =
        NGameEngine::CreateRotatingCamera({0, 0, 0}, glm::radians(30.f), 20.f);
    engine_->bindCamera(camera_.get());
}

void TGame::update(float dt) {
    using namespace std::chrono_literals;

    std::this_thread::sleep_for(16ms);
}

}  // namespace NGachiBall
