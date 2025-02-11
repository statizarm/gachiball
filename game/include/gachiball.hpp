#pragma once

#include <vector>

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
    void restart();

  private:
    NGameEngine::TBody platform_;
    NGameEngine::TBody ball_;

    std::vector<std::unique_ptr<NGameEngine::IMesh>> meshes_;

    std::unique_ptr<NGameEngine::ICamera> camera_;

    NGameEngine::TGameEngine* engine_;
};

}  // namespace NGachiBall
