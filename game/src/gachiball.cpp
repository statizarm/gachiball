#include "gachiball.hpp"

#include <glm/trigonometric.hpp>
#include <thread>

namespace NGachiBall {

TGame::TGame(NGameEngine::TGameEngine* engine) : engine_(engine) {}

void TGame::deinit() {
    camera_.reset();

    engine_->removeBody(&platform_);
    engine_->removeBody(&ball_);

    platform_ = NGameEngine::TBody{};
    ball_ = NGameEngine::TBody{};
}

void TGame::init() {
    assert(!camera_);

    meshes_.resize(2);
    if (!meshes_[0]) {
        meshes_[0] = NGameEngine::CreatePlatformMesh();
    }
    if (!meshes_[1]) {
        meshes_[1] = NGameEngine::CreateBallMesh();
    }

    platform_ = NGameEngine::TBody{
        .mesh = meshes_[0].get(),
        .position = {0.f, 0.f, 0.f},
    };
    engine_->addBody(&platform_);

    ball_ = NGameEngine::TBody{
        .mesh = meshes_[1].get(),
        .position = {0.f, 5.f, 0.f},
    };
    engine_->addBody(&ball_);

    camera_ =
        NGameEngine::CreateRotatingCamera({0, 0, 0}, glm::radians(30.f), 20.f);
    engine_->bindCamera(camera_.get());

    initKeyMap();
}

void TGame::update(float dt) {
    using namespace std::chrono_literals;

    std::this_thread::sleep_for(16ms);
}

void TGame::restart() {
    deinit();
    init();
}

void TGame::initKeyMap() {
    using namespace NGameEngine;

    engine_->registerInputCallback(
        NGameEngine::TInputEventType{
            .input_device = EInputDevice::KEYBOARD,
            .key = EKey::SPACE,
            .key_action = EKeyAction::PRESSED,
        },
        [this](TInputEvent) { this->restart(); }
    );
}

}  // namespace NGachiBall
