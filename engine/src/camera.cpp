#include "camera.hpp"

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

namespace NCamera {

namespace {

class TRotatingCamera : public ICamera {
  public:
    TRotatingCamera(glm::mat4x4 view);
    ~TRotatingCamera() override = default;

    glm::mat4x4 view() const override;

  private:
    glm::mat4x4 initView_;
    double initTime_;
};

TRotatingCamera::TRotatingCamera(glm::mat4x4 view)
    : initView_(view), initTime_(glfwGetTime()) {}

glm::mat4x4 TRotatingCamera::view() const {
    float angle = glfwGetTime() - initTime_;

    return glm::rotate(initView_, angle, {0.f, 1.f, 0.f});
}

}  // namespace

std::unique_ptr<NCamera::ICamera> CreateRotatingCamera(
    glm::vec3 lookTo, float angle, float distance
) {
    auto view = glm::lookAt(
        lookTo +
            glm::vec3{
                0, glm::sin(angle) * distance, glm::cos(angle) * distance
            },
        lookTo, glm::vec3{0.f, 1.f, 0.f}
    );

    return std::make_unique<TRotatingCamera>(std::move(view));
}

}  // namespace NCamera
