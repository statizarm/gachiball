#pragma once

#include <functional>
#include <memory>

#include "body.hpp"
#include "camera.hpp"
#include "game.hpp"

namespace NGameEngine {

using TInputCallback = std::function<void()>;

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

    void registerInputCallback(TInputCallback callback);

  private:
    std::unique_ptr<TGameEngineImpl> impl_;
};

}  // namespace NGameEngine
