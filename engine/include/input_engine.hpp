#pragma once

#include <GLFW/glfw3.h>

#include <memory>

#include "event_dispatcher.hpp"

namespace NGameEngine {

class TInputEngine {
  public:
    class TImpl;

  public:
    TInputEngine();
    ~TInputEngine();

    // register callbacks that calls game_engine handlers
    void init(GLFWwindow* window, TEventDispatcher* event_dispatcher);
    // unregister callbacks
    void deinit();

  private:
    std::unique_ptr<TImpl> impl_;
};

}  // namespace NGameEngine
