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
    void run(NGameEngine::IGame* game);

    void bindCamera(const NGameEngine::ICamera* camera);

  private:
    std::unique_ptr<TGameEngineImpl> impl_;
};

}  // namespace NGameEngine
