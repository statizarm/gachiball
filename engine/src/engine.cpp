#include "engine.hpp"

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <unordered_set>

#include "event_dispatcher.hpp"
#include "input_engine.hpp"
#include "mesh.hpp"

namespace NGameEngine {

class TGameEngineImpl {
  public:
    TGameEngineImpl() = default;

    void init();
    void deinit();
    void run(IGame *game);

    void bindCamera(const ICamera *camera);

    void addBody(TBody *body);
    void removeBody(TBody *body);

    void registerInputCallback(
        TInputEventType event_type, TInputCallback callback
    );

  public:
    // NOTE: Various callbacks
    void frameBufferSizeCallback(GLFWwindow *window, int width, int height);

  private:
    GLFWwindow *window_;
    int window_width_;
    int window_height_;

    TInputEngine input_engine_;
    TEventDispatcher event_dispatcher_;

    std::unordered_set<TBody *> bodies_;
    const ICamera *camera_;
};

static TGameEngineImpl *game_engine = nullptr;

static void ErrorCallback(int error, const char *description) {
    std::cerr << "Error: %s\n" << description << std::endl;
}

static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    assert(game_engine);

    game_engine->frameBufferSizeCallback(window, width, height);
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

    glfwGetWindowSize(window_, &window_width_, &window_height_);
    input_engine_.init(window_, &event_dispatcher_);

    game_engine = this;
}

void TGameEngineImpl::deinit() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
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

        auto view       = camera_->view();
        auto projection = glm::perspective(
            glm::radians(45.f),
            static_cast<float>(window_width_) /
                static_cast<float>(window_height_),
            0.1f,
            100.f
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
        game->update(duration);

        start = glfwGetTime();
    }
    game->deinit();
}

void TGameEngineImpl::bindCamera(const ICamera *camera) {
    camera_ = camera;
}

void TGameEngineImpl::addBody(TBody *body) {
    bodies_.insert(body);
}

void TGameEngineImpl::removeBody(TBody *body) {
    bodies_.erase(body);
}

void TGameEngineImpl::registerInputCallback(
    TInputEventType event_type, TInputCallback callback
) {
    event_dispatcher_.registerEventHandler(
        MakeEventType(std::move(event_type)),
        [callback = std::move(callback)](TEvent event) {
            callback(std::get<TInputEvent>(event));
        }
    );
}

void TGameEngineImpl::frameBufferSizeCallback(
    GLFWwindow *window, int width, int height
) {
    glViewport(0, 0, width, height);
    window_width_  = width;
    window_height_ = height;
}

TGameEngine::TGameEngine() {
}

TGameEngine::~TGameEngine() {
}

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

void TGameEngine::registerInputCallback(
    TInputEventType event_type, TInputCallback callback
) {
    assert(impl_);

    impl_->registerInputCallback(std::move(event_type), std::move(callback));
}

};  // namespace NGameEngine
