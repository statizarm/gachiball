#pragma once

#include "camera.hpp"

namespace NGachiBall {

class TPlayerCamera : public NGameEngine::ICamera {
  public:
    TPlayerCamera(glm::vec3 look_to);
    ~TPlayerCamera() = default;

    glm::mat4x4 view() const override;

    void move(float x_delta, float y_delta);
    void reset();

  private:
    glm::vec3 look_to_;

    float distance_;
    float alpha_;
    float theta_;
};

}  // namespace NGachiBall
