#pragma once

#include <functional>
#include <memory>

#include "event.hpp"

namespace NGameEngine {

using TEventHandler = std::function<void(TEvent)>;

class TEventDispatcher {
    class TImpl;

  public:
    TEventDispatcher();
    ~TEventDispatcher();

  public:
    void registerEventHandler(
        TEventType event_type, TEventHandler event_handler
    );
    void raiseEvent(TEvent event);

  private:
    std::unique_ptr<TImpl> impl_;
};

}  // namespace NGameEngine
