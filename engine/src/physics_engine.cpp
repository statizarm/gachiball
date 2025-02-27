#include "physics_engine.hpp"

namespace NGameEngine {

void TPhysicsEngine::init(float simulation_step) {
    spent_time_      = 0.f;
    simulation_step_ = simulation_step;
}

void TPhysicsEngine::deinit() {
    spent_time_      = 0.f;
    simulation_step_ = 0.f;

    bodies_.clear();
}

void TPhysicsEngine::update(float dt) {
    spent_time_ += dt;
    if (spent_time_ > simulation_step_) {
        simulate(spent_time_);
        spent_time_ = 0;
    }
}

void TPhysicsEngine::addRigidBody(TRigidBody* body) {
    bodies_.insert(body);
}

void TPhysicsEngine::simulate(float dt) {
    moveBodies(dt);
    applyForces(dt);
}

void TPhysicsEngine::moveBodies(float dt) {
    for (auto* body : bodies_) {
        body->position += body->velocity * dt;
    }
}

void TPhysicsEngine::applyForces(float dt) {
    static constexpr auto kG = 10.f * glm::vec3{0.f, -1.f, 0.f};

    for (auto* body : bodies_) {
        body->acceleration = body->mass * kG * body->mass_inv;
        body->velocity += body->acceleration * dt;
    }
}

}  // namespace NGameEngine
