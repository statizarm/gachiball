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
#include "physics_engine.hpp"
#include "window.hpp"

namespace NGameEngine {

class TGameEngineImpl {
  public:
    TGameEngineImpl() = default;

    void init();
    void deinit();
    void run(IGame *game);

    void bindCamera(const ICamera *camera);

    void addBody(TBody *body);
    void addBody(TRigidBody *body);
    void removeBody(TBody *body);

    void registerInputCallback(
        TInputEventType event_type, TInputCallback callback
    );
    void unregisterInputCallback(TInputEventType event_type);

  public:
    // NOTE: Various callbacks
    void frameBufferSizeCallback(GLFWwindow *window, int width, int height);

  private:
    std::unique_ptr<TWindow> window_;

    TInputEngine input_engine_;
    TEventDispatcher event_dispatcher_;
    TPhysicsEngine physics_engine_;

    std::unordered_set<TBody *> bodies_;
    const ICamera *camera_;
};

void TGameEngineImpl::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw" << std::endl;
        std::exit(1);
    }

    window_ = TWindow::MakeGLFWWindow();
    if (!window_) {
        std::cerr << "Failed to create window" << std::endl;
    }
    window_->bindCurrentContext();

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad" << std::endl;
        std::exit(5);
    }

    input_engine_.init(window_.get(), &event_dispatcher_);
    physics_engine_.init(1.f / 60.f);
}

void TGameEngineImpl::deinit() {
    window_.reset();
    physics_engine_.deinit();
    glfwTerminate();
}

void TGameEngineImpl::run(IGame *game) {
    glEnable(GL_DEPTH_TEST);
    game->init();
    auto start = glfwGetTime();
    while (!window_->shouldClose()) {
        ///////////////////////////////////////////////////////////////////////
        // NOTE: DRAW
        auto [width, height] = window_->window_size();
        glViewport(0, 0, width, height);
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto projection = glm::perspective(
            glm::radians(45.f),
            static_cast<float>(width) / static_cast<float>(height),
            0.1f,
            100.f
        );
        auto vp = projection * camera_->view();

        for (const auto body : bodies_) {
            auto model =
                glm::translate(glm::mat4_cast(body->rotation), body->position);
            body->mesh->draw(vp * model);
        }

        window_->swapBuffers();
        glfwPollEvents();

        auto duration = glfwGetTime() - start;
        ///////////////////////////////////////////////////////////////////////
        // NOTE: update physics
        physics_engine_.update(duration);

        ///////////////////////////////////////////////////////////////////////
        // NOTE: update game

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

void TGameEngineImpl::addBody(TRigidBody *body) {
    physics_engine_.addRigidBody(body);
    addBody(static_cast<TBody *>(body));
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

void TGameEngineImpl::unregisterInputCallback(TInputEventType event_type) {
    event_dispatcher_.registerEventHandler(
        MakeEventType(std::move(event_type)), TEventHandler{}
    );
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

void TGameEngine::addBody(TRigidBody *body) {
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

void TGameEngine::unregisterInputCallback(TInputEventType event_type) {
    assert(impl_);
    impl_->unregisterInputCallback(std::move(event_type));
}

};  // namespace NGameEngine
