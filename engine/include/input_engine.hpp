#pragma once

#include <memory>

#include "event_dispatcher.hpp"
#include "window.hpp"

namespace NGameEngine {

class TInputEngine {
  public:
    class TImpl;

  public:
    TInputEngine();
    ~TInputEngine();

    // register callbacks
    void init(TWindow* window, TEventDispatcher* event_dispatcher);
    // unregister callbacks
    void deinit();

  private:
    std::unique_ptr<TImpl> impl_;
};

}  // namespace NGameEngine
