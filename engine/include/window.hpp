#pragma once

#include <memory>

namespace NGameEngine {

using TKeyboardKeyCallback =
    void(void*, int key, int scancode, int action, int mods);
using TMouseKeyCallback       = void(void*, int key, int action, int mods);
using TCursorPositionCallback = void(void*, double xpos, double ypos);

class TWindowImpl;

class TWindow {
  public:
    TWindow();
    ~TWindow();

  public:
    static std::unique_ptr<TWindow> MakeGLFWWindow();

  public:
    // getters
    int width() const;
    int height() const;
    std::pair<int, int> window_size() const;

    std::pair<double, double> cursor_position() const;

  public:
    bool shouldClose();

    void bindCurrentContext();
    void swapBuffers();

    void grabCursor();
    void ungrabCursor();

    void registerKeyboardKeyCallback(TKeyboardKeyCallback callback);
    void registerMouseKeyCallback(TMouseKeyCallback callback);
    void registerCursorPositionCallback(TCursorPositionCallback callback);

  private:
    TWindow(std::unique_ptr<TWindowImpl> impl);

  private:
    std::unique_ptr<TWindowImpl> impl_;
};

}  // namespace NGameEngine
