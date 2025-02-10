#pragma once

#include <memory>

#include "camera.hpp"
#include "game.hpp"

namespace NGameEngine {

namespace {
class TGameEngineImpl;
}

class TGameEngine {
  public:
    TGameEngine();
    ~TGameEngine();

    void init();
    void deinit();
    void run(NGame::IGame* game);

    void bindCamera(const NCamera::ICamera* camera);

  private:
    std::unique_ptr<TGameEngineImpl> impl_;
};

}  // namespace NGameEngine
