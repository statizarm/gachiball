#pragma once

namespace NGameEngine {

class IGame {
  public:
    virtual ~IGame() = default;

    virtual void update(float dt) = 0;

    virtual void init();
    virtual void deinit();
};

}  // namespace NGameEngine
