#include "player_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace NGachiBall {

static constexpr float kDefaultDistance = 20.f;
static constexpr float kInitXAngle      = glm::radians(30.f);
static constexpr float kInitYAngle      = glm::radians(0.f);
static constexpr float kRotationSpeed   = 1.f;

TPlayerCamera::TPlayerCamera(glm::vec3 look_to)
    : look_to_(std::move(look_to))
    , distance_(kDefaultDistance)
    , x_angle_(kInitXAngle)
    , y_angle_(kInitYAngle) {
}

glm::mat4x4 TPlayerCamera::view() const {
    glm::vec3 position =
        distance_ * glm::vec3{
                        glm::sin(y_angle_) * glm::cos(x_angle_),
                        glm::sin(x_angle_),
                        glm::cos(y_angle_) * glm::cos(x_angle_)
                    };

    glm::vec3 up = glm::vec3{0.f, 1.f, 0.f};

    return glm::lookAt(position, look_to_, up);
}

void TPlayerCamera::move(float x_delta, float y_delta) {
    x_angle_ += glm::radians(x_delta);
    y_angle_ += glm::radians(y_delta);
}

void TPlayerCamera::reset() {
    x_angle_ = kInitXAngle;
    y_angle_ = kInitYAngle;
}

}  // namespace NGachiBall
