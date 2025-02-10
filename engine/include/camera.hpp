#pragma once

#include <glm/mat4x4.hpp>
#include <memory>

namespace NGameEngine {

class ICamera {
  public:
    virtual ~ICamera() = default;
    virtual glm::mat4x4 view() const = 0;
};

std::unique_ptr<ICamera> CreateRotatingCamera(
    glm::vec3 lookTo, float angle, float distance
);

}  // namespace NGameEngine
