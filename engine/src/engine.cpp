#include "engine.hpp"

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "mesh.hpp"

namespace NGameEngine {

namespace {

class TGameEngineImpl {
  public:
    TGameEngineImpl() = default;

    void init();
    void deinit();
    void run(IGame *game);

    void bindCamera(const ICamera *camera);

    void addBody(TBody *body);
    void removeBody(TBody *body);

    void registerInputCallback(TInputCallback callback);

  public:
    // NOTE: Various callbacks
    void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
    void keyCallback(
        GLFWwindow *window, int key, int scancode, int action, int mods
    );

  private:
    GLFWwindow *window_;
    int window_width_;
    int window_height_;

    TInputCallback input_callback_;

    std::unordered_set<TBody *> bodies_;
    const ICamera *camera_;
};

static TGameEngineImpl *gameEngine = nullptr;

static void ErrorCallback(int error, const char *description) {
    std::cerr << "Error: %s\n" << description << std::endl;
}

static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    assert(gameEngine);

    gameEngine->frameBufferSizeCallback(window, width, height);
}

static void KeyCallback(
    GLFWwindow *window, int key, int scancode, int action, int mods
) {
    assert(gameEngine);

    gameEngine->keyCallback(window, key, scancode, action, mods);
}

void TGameEngineImpl::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw" << std::endl;
        std::exit(1);
    }

    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    window_ = glfwCreateWindow(640, 480, "GachiBall", NULL, NULL);
    if (!window_) {
        std::cerr << "Failed to create window" << std::endl;
        std::exit(2);
    }

    glfwMakeContextCurrent(window_);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad" << std::endl;
        std::exit(5);
    }

    glfwSetFramebufferSizeCallback(window_, FrameBufferSizeCallback);

    glfwSetKeyCallback(window_, KeyCallback);

    gameEngine = this;
    glfwGetWindowSize(window_, &window_width_, &window_height_);
}

void TGameEngineImpl::deinit() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();

    gameEngine = nullptr;
}

void TGameEngineImpl::run(IGame *game) {
    glEnable(GL_DEPTH_TEST);
    game->init();
    auto start = glfwGetTime();
    while (!glfwWindowShouldClose(window_)) {
        ///////////////////////////////////////////////////////////////////////
        // NOTE: DRAW
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = camera_->view();
        auto projection = glm::perspective(
            glm::radians(45.f),
            static_cast<float>(window_width_) /
                static_cast<float>(window_height_),
            0.1f, 100.f
        );
        auto vp = projection * view;

        for (const auto body : bodies_) {
            auto model =
                glm::translate(glm::mat4_cast(body->rotation), body->position);
            body->mesh->draw(vp * model);
        }

        glfwSwapBuffers(window_);
        glfwPollEvents();

        ///////////////////////////////////////////////////////////////////////
        // NOTE: update game

        auto duration = glfwGetTime() - start;
        start = glfwGetTime();
        game->update(duration);
    }
    game->deinit();
}

void TGameEngineImpl::bindCamera(const ICamera *camera) { camera_ = camera; }

void TGameEngineImpl::addBody(TBody *body) { bodies_.insert(body); }

void TGameEngineImpl::removeBody(TBody *body) { bodies_.erase(body); }

void TGameEngineImpl::registerInputCallback(TInputCallback callback) {
    input_callback_ = std::move(callback);
}

void TGameEngineImpl::frameBufferSizeCallback(
    GLFWwindow *window, int width, int height
) {
    glViewport(0, 0, width, height);
    window_width_ = width;
    window_height_ = height;
}

void TGameEngineImpl::keyCallback(
    GLFWwindow *window, int key, int scancode, int action, int mods
) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        input_callback_();
    }
}

}  // namespace

TGameEngine::TGameEngine() {}

TGameEngine::~TGameEngine() {}

void TGameEngine::init() {
    assert(!impl_);

    impl_ = std::make_unique<TGameEngineImpl>();
    impl_->init();
}

void TGameEngine::deinit() {
    assert(impl_);

    impl_.reset();
}

void TGameEngine::run(IGame *game) {
    assert(impl_);

    impl_->run(game);
}

void TGameEngine::bindCamera(const ICamera *camera) {
    assert(impl_);

    impl_->bindCamera(camera);
}

void TGameEngine::addBody(TBody *body) {
    assert(impl_);

    impl_->addBody(body);
}

void TGameEngine::removeBody(TBody *body) {
    assert(impl_);

    impl_->removeBody(body);
}

void TGameEngine::registerInputCallback(TInputCallback callback) {
    assert(impl_);

    impl_->registerInputCallback(std::move(callback));
}
};  // namespace NGameEngine
