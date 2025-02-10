#pragma once

#include "engine.hpp"
#include "game.hpp"

namespace NGachiBall {

class TGame : public NGameEngine::IGame {
  public:
    TGame(NGameEngine::TGameEngine* engine);
    ~TGame() override = default;

    void init() override;
    void update(float dt) override;
    void deinit() override;

  private:
    std::unique_ptr<NGameEngine::ICamera> camera_;

    NGameEngine::TGameEngine* engine_;
};

}  // namespace NGachiBall
