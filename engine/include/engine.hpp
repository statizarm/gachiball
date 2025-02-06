#pragma once

#include <memory>

#include "camera.hpp"

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

    void bindCamera(const NCamera::ICamera* camera_);

  private:
    std::unique_ptr<TGameEngineImpl> impl_;
};

}  // namespace NGameEngine
