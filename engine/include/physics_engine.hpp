#pragma once

#include <unordered_set>

#include "body.hpp"

namespace NGameEngine {

class TPhysicsEngine {
  public:
    TPhysicsEngine()  = default;
    ~TPhysicsEngine() = default;

    void init(float simulation_step);
    void deinit();

    void addRigidBody(TRigidBody* rigid_body);
    void update(float dt);

  private:
    void simulate(float dt);

    void moveBodies(float dt);
    void applyForces(float dt);

  private:
    float simulation_step_;
    float spent_time_;
    std::unordered_set<TRigidBody*> bodies_;
};

}  // namespace NGameEngine
