#include "window.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

namespace NGameEngine {

class TWindowImpl {
  public:
    virtual ~TWindowImpl() = default;

  public:
    virtual std::pair<int, int> window_size() const           = 0;
    virtual std::pair<double, double> cursor_position() const = 0;

  public:
    virtual bool shouldClose() = 0;

    virtual void bindCurrentContext() = 0;
    virtual void swapBuffers()        = 0;

    virtual void grabCursor()   = 0;
    virtual void ungrabCursor() = 0;

    virtual void registerKeyboardKeyCallback(TKeyboardKeyCallback callback) = 0;
    virtual void registerMouseKeyCallback(TMouseKeyCallback callback)       = 0;
    virtual void registerCursorPositionCallback(TCursorPositionCallback callback
    )                                                                       = 0;
};

class TGLFWWindow : public TWindowImpl {
  public:
    TGLFWWindow(GLFWwindow* window);
    ~TGLFWWindow();

  public:
    std::pair<int, int> window_size() const override;
    virtual std::pair<double, double> cursor_position() const override;

  public:
    bool shouldClose() override;

    void bindCurrentContext() override;
    void swapBuffers() override;

    void grabCursor() override;
    void ungrabCursor() override;

    void registerKeyboardKeyCallback(TKeyboardKeyCallback callback) override;
    void registerMouseKeyCallback(TMouseKeyCallback callback) override;
    void registerCursorPositionCallback(TCursorPositionCallback callback
    ) override;

  private:
    GLFWwindow* window_;
};

TGLFWWindow::TGLFWWindow(GLFWwindow* window)
    : window_(window) {
}

TGLFWWindow::~TGLFWWindow() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
}

std::pair<int, int> TGLFWWindow::window_size() const {
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    return {width, height};
}

std::pair<double, double> TGLFWWindow::cursor_position() const {
    double xpos, ypos;
    glfwGetCursorPos(window_, &xpos, &ypos);
    return {xpos, ypos};
}

bool TGLFWWindow::shouldClose() {
    return glfwWindowShouldClose(window_);
}

void TGLFWWindow::bindCurrentContext() {
    glfwMakeContextCurrent(window_);
}

void TGLFWWindow::swapBuffers() {
    glfwSwapBuffers(window_);
}

void TGLFWWindow::grabCursor() {
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void TGLFWWindow::ungrabCursor() {
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void TGLFWWindow::registerKeyboardKeyCallback(TKeyboardKeyCallback callback) {
    glfwSetKeyCallback(window_, reinterpret_cast<GLFWkeyfun>(callback));
}

void TGLFWWindow::registerMouseKeyCallback(TMouseKeyCallback callback) {
    glfwSetMouseButtonCallback(
        window_, reinterpret_cast<GLFWmousebuttonfun>(callback)
    );
}

void TGLFWWindow::registerCursorPositionCallback(
    TCursorPositionCallback callback
) {
    glfwSetCursorPosCallback(
        window_, reinterpret_cast<GLFWcursorposfun>(callback)
    );
}

static void ErrorCallback(int error, const char* description) {
    std::cerr << "Error: %s\n" << description << std::endl;
}

std::unique_ptr<TWindow> TWindow::MakeGLFWWindow() {
    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    auto window = glfwCreateWindow(640, 480, "GachiBall", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        return nullptr;
    }

    TWindow* w = new TWindow(std::make_unique<TGLFWWindow>(window));
    return std::unique_ptr<TWindow>{w};
}

TWindow::TWindow() {
}

TWindow::~TWindow() {
}

int TWindow::width() const {
    return window_size().first;
}

int TWindow::height() const {
    return window_size().second;
}

std::pair<int, int> TWindow::window_size() const {
    return impl_->window_size();
}

std::pair<double, double> TWindow::cursor_position() const {
    return impl_->cursor_position();
}

bool TWindow::shouldClose() {
    return impl_->shouldClose();
}

void TWindow::bindCurrentContext() {
    return impl_->bindCurrentContext();
}

void TWindow::swapBuffers() {
    return impl_->swapBuffers();
}

void TWindow::grabCursor() {
    impl_->grabCursor();
}

void TWindow::ungrabCursor() {
    impl_->ungrabCursor();
}

void TWindow::registerKeyboardKeyCallback(TKeyboardKeyCallback callback) {
    impl_->registerKeyboardKeyCallback(std::move(callback));
}

void TWindow::registerMouseKeyCallback(TMouseKeyCallback callback) {
    impl_->registerMouseKeyCallback(std::move(callback));
}

void TWindow::registerCursorPositionCallback(TCursorPositionCallback callback) {
    impl_->registerCursorPositionCallback(std::move(callback));
}

TWindow::TWindow(std::unique_ptr<TWindowImpl> impl)
    : impl_(std::move(impl)) {
}

}  // namespace NGameEngine
