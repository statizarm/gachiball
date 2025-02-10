#pragma once

#include "engine.hpp"
#include "game.hpp"

namespace NGachiBall {

class TGame : public NGame::IGame {
  public:
    TGame(NGameEngine::TGameEngine* engine);
    ~TGame() override = default;

    void init() override;
    void update(float dt) override;
    void deinit() override;

  private:
    std::unique_ptr<NCamera::ICamera> camera_;

    NGameEngine::TGameEngine* engine_;
};

}  // namespace NGachiBall
