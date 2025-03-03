#pragma once

#include <vector>

#include "engine.hpp"
#include "game.hpp"
#include "player_camera.hpp"

namespace NGachiBall {

class TGame : public NGameEngine::IGame {
  public:
    TGame(NGameEngine::TGameEngine* engine);
    ~TGame() override = default;

    void init() override;
    void update(float dt) override;
    void deinit() override;

  private:
    void restart();
    void lose();
    void win();

    void initKeyMap();

  private:
    NGameEngine::TRigidBody platform_;
    NGameEngine::TRigidBody ball_;

    std::vector<std::unique_ptr<NGameEngine::IMesh>> meshes_;

    std::unique_ptr<TPlayerCamera> camera_;

    NGameEngine::TGameEngine* engine_;

    int z_rotation_factor_ = 0;
    int x_rotation_factor_ = 0;
};

}  // namespace NGachiBall
