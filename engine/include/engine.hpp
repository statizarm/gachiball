#pragma once

#include <memory>

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
    void run();

  private:
    std::unique_ptr<TGameEngineImpl> impl_;
};

}  // namespace NGameEngine
