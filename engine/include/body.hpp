#pragma once

#include <glm/gtc/quaternion.hpp>

#include "mesh.hpp"

namespace NGameEngine {

struct TBody {
    // NOTE: for drawing only
    IMesh* mesh;

    // NOTE: for drawing ang physics
    glm::vec3 position;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    glm::quat rotation;
};

struct TRigidBody : public TBody {
    float mass;
    float mass_inv;
};

}  // namespace NGameEngine
