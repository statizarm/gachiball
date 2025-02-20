#include "gachiball.hpp"

#include <glm/trigonometric.hpp>

#include "player_camera.hpp"

namespace NGachiBall {

TGame::TGame(NGameEngine::TGameEngine* engine)
    : engine_(engine) {
}

void TGame::deinit() {
    camera_.reset();

    engine_->removeBody(&platform_);
    engine_->removeBody(&ball_);

    platform_ = NGameEngine::TBody{};
    ball_     = NGameEngine::TBody{};
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
        .mesh     = meshes_[0].get(),
        .position = {0.f, 0.f, 0.f},
        .rotation = glm::quat_cast(glm::identity<glm::mat4x4>()),
    };
    engine_->addBody(&platform_);

    ball_ = NGameEngine::TBody{
        .mesh     = meshes_[1].get(),
        .position = {0.f, 5.f, 0.f},
        .rotation = glm::quat_cast(glm::identity<glm::mat4x4>()),
    };
    engine_->addBody(&ball_);

    camera_ = std::make_unique<NGachiBall::TPlayerCamera>(glm::vec3{0, 0, 0});
    engine_->bindCamera(camera_.get());

    initKeyMap();
}

void TGame::update(float dt) {
    constexpr float kRotationSpeed = 1.f;

    platform_.rotation = glm::rotate(
        platform_.rotation,
        kRotationSpeed * dt * x_rotation_factor_,
        {1.f, 0.f, 0.f}
    );
    platform_.rotation = glm::rotate(
        platform_.rotation,
        kRotationSpeed * dt * z_rotation_factor_,
        {0.f, 0.f, 1.f}
    );
}

void TGame::restart() {
    deinit();
    init();
}

void TGame::initKeyMap() {
    using namespace NGameEngine;
    // TODO: replace this copy paste with .txt file and function aliases

    engine_->registerInputCallback(
        NGameEngine::TInputEventType{
            .input_device = EInputDevice::KEYBOARD,
            .key          = EKey::KEY_SPACE,
            .key_action   = EKeyAction::PRESSED,
        },
        [this](TInputEvent) { this->restart(); }
    );

    // Rotation around Z axis
    engine_->registerInputCallback(
        NGameEngine::TInputEventType{
            .input_device = EInputDevice::KEYBOARD,
            .key          = EKey::KEY_A,
            .key_action   = EKeyAction::PRESSED,
        },
        [this](TInputEvent) { this->z_rotation_factor_ += 1; }
    );
    engine_->registerInputCallback(
        NGameEngine::TInputEventType{
            .input_device = EInputDevice::KEYBOARD,
            .key          = EKey::KEY_A,
            .key_action   = EKeyAction::RELEASED,
        },
        [this](TInputEvent) { this->z_rotation_factor_ -= 1; }
    );
    engine_->registerInputCallback(
        NGameEngine::TInputEventType{
            .input_device = EInputDevice::KEYBOARD,
            .key          = EKey::KEY_D,
            .key_action   = EKeyAction::PRESSED,
        },
        [this](TInputEvent) { this->z_rotation_factor_ -= 1; }
    );
    engine_->registerInputCallback(
        NGameEngine::TInputEventType{
            .input_device = EInputDevice::KEYBOARD,
            .key          = EKey::KEY_D,
            .key_action   = EKeyAction::RELEASED,
        },
        [this](TInputEvent) { this->z_rotation_factor_ += 1; }
    );

    // Rotation around X axis
    engine_->registerInputCallback(
        NGameEngine::TInputEventType{
            .input_device = EInputDevice::KEYBOARD,
            .key          = EKey::KEY_S,
            .key_action   = EKeyAction::PRESSED,
        },
        [this](TInputEvent) { this->x_rotation_factor_ += 1; }
    );
    engine_->registerInputCallback(
        NGameEngine::TInputEventType{
            .input_device = EInputDevice::KEYBOARD,
            .key          = EKey::KEY_S,
            .key_action   = EKeyAction::RELEASED,
        },
        [this](TInputEvent) { this->x_rotation_factor_ -= 1; }
    );
    engine_->registerInputCallback(
        NGameEngine::TInputEventType{
            .input_device = EInputDevice::KEYBOARD,
            .key          = EKey::KEY_W,
            .key_action   = EKeyAction::PRESSED,
        },
        [this](TInputEvent) { this->x_rotation_factor_ -= 1; }
    );
    engine_->registerInputCallback(
        NGameEngine::TInputEventType{
            .input_device = EInputDevice::KEYBOARD,
            .key          = EKey::KEY_W,
            .key_action   = EKeyAction::RELEASED,
        },
        [this](TInputEvent) { this->x_rotation_factor_ += 1; }
    );

    // Camera control
    engine_->registerInputCallback(
        NGameEngine::TInputEventType{
            .input_device = EInputDevice::MOUSE,
            .key          = EKey::MOUSE_LEFT,
            .key_action   = EKeyAction::PRESSED,
        },
        [this](TInputEvent) {}
    );
    engine_->registerInputCallback(
        NGameEngine::TInputEventType{
            .input_device = EInputDevice::MOUSE,
            .key          = EKey::MOUSE_LEFT,
            .key_action   = EKeyAction::RELEASED,
        },
        [this](TInputEvent) {}
    );
}

}  // namespace NGachiBall
