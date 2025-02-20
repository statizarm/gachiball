#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace NGameEngine {

struct TMeshData {
    glm::vec3 position;
    glm::vec4 color;
};

class IMesh {
  public:
    virtual ~IMesh() = default;

    virtual void draw(const glm::mat4& mvp) = 0;
};

std::unique_ptr<IMesh> CreatePlatformMesh();
std::unique_ptr<IMesh> CreateBallMesh();

}  // namespace NGameEngine
