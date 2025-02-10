#include "engine.hpp"

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "mesh.hpp"

namespace NGameEngine {

static void ErrorCallback(int error, const char *description) {
    std::cerr << "Error: %s\n" << description << std::endl;
}

static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

namespace {

class TGameEngineImpl {
  public:
    TGameEngineImpl() = default;

    void init();
    void deinit();
    void run(NGame::IGame *game);

    void bindCamera(const NCamera::ICamera *camera);

  private:
    GLFWwindow *window_;

    std::vector<std::unique_ptr<NMesh::IMesh>> meshes_;
    const NCamera::ICamera *camera_;
};

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
}

void TGameEngineImpl::deinit() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
}

void TGameEngineImpl::run(NGame::IGame *game) {
    meshes_.emplace_back(std::move(NMesh::CreatePlatformMesh()));
    meshes_.emplace_back(std::move(NMesh::CreateBallMesh()));

    glEnable(GL_DEPTH_TEST);
    game->init();
    auto start = glfwGetTime();
    while (!glfwWindowShouldClose(window_)) {
        ///////////////////////////////////////////////////////////////////////
        // NOTE: DRAW
        int width, height;
        glfwGetWindowSize(window_, &width, &height);

        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // auto model = glm::identity<glm::mat4x4>();
        auto view = camera_->view();
        auto projection = glm::perspective(
            glm::radians(45.f),
            static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.f
        );
        auto mvp = projection * view;

        for (const auto &mesh : meshes_) {
            mesh->draw(mvp);
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

void TGameEngineImpl::bindCamera(const NCamera::ICamera *camera) {
    camera_ = camera;
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

void TGameEngine::run(NGame::IGame *game) {
    assert(impl_);

    impl_->run(game);
}

void TGameEngine::bindCamera(const NCamera::ICamera *camera) {
    assert(impl_);

    impl_->bindCamera(camera);
}

};  // namespace NGameEngine
