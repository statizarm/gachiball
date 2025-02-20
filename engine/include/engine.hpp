#pragma once

#include <functional>
#include <memory>

#include "body.hpp"
#include "camera.hpp"
#include "game.hpp"
#include "input_event.hpp"

namespace NGameEngine {

using TInputCallback = std::function<void(TInputEvent)>;

class TGameEngineImpl;

class TGameEngine {
  public:
    TGameEngine();
    ~TGameEngine();

    void init();
    void deinit();
    void run(IGame* game);

  public:
    void bindCamera(const ICamera* camera);

    void addBody(TBody* body);
    void removeBody(TBody* body);

    void registerInputCallback(
        TInputEventType inputEventType, TInputCallback callback
    );
    void unregisterInputCallback(TInputEventType inputEventType);

  private:
    std::unique_ptr<TGameEngineImpl> impl_;
};

}  // namespace NGameEngine
