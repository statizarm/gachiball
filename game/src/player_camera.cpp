#include "player_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace NGachiBall {

static constexpr float kDefaultDistance = 20.f;
static constexpr float kInitAlpha       = glm::radians(0.f);
static constexpr float kInitTheta       = glm::radians(30.f);
static constexpr float kRotationSpeed   = 0.05f;

TPlayerCamera::TPlayerCamera(glm::vec3 look_to)
    : look_to_(std::move(look_to))
    , distance_(kDefaultDistance)
    , alpha_(kInitAlpha)
    , theta_(kInitTheta) {
}

glm::mat4x4 TPlayerCamera::view() const {
    glm::vec3 position = distance_ * glm::vec3{
                                         glm::sin(alpha_) * glm::cos(theta_),
                                         glm::sin(theta_),
                                         glm::cos(alpha_) * glm::cos(theta_)
                                     };

    glm::vec3 up = glm::vec3{0.f, 1.f, 0.f};

    return glm::lookAt(position, look_to_, up);
}

void TPlayerCamera::move(float x_delta, float y_delta) {
    alpha_ -= glm::radians(x_delta) * kRotationSpeed;
    theta_ += glm::radians(y_delta) * kRotationSpeed;
}

void TPlayerCamera::reset() {
    alpha_ = kInitAlpha;
    theta_ = kInitTheta;
}

}  // namespace NGachiBall
