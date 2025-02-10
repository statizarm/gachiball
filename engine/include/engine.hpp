#pragma once

#include <memory>

#include "body.hpp"
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
    void run(IGame* game);

    void bindCamera(const ICamera* camera);

    void addBody(TBody* body);
    void removeBody(TBody* body);

  private:
    std::unique_ptr<TGameEngineImpl> impl_;
};

}  // namespace NGameEngine
