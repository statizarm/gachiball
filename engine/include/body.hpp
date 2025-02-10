#include <glm/gtc/quaternion.hpp>

#include "mesh.hpp"

namespace NGameEngine {

struct TBody {
    // NOTE: for drawing only
    IMesh* mesh;

    // NOTE: for drawing ang physics
    glm::vec3 position;
    glm::quat rotation;
};

}  // namespace NGameEngine
