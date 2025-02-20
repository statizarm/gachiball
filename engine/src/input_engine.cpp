#include "input_engine.hpp"

#include <cassert>

namespace NGameEngine {

namespace {

static void KeyCallback(
    GLFWwindow* window, int key, int scancode, int action, int mods
);

}  // namespace

class TInputEngine::TImpl {
  public:
    TImpl(GLFWwindow* window, TEventDispatcher* event_dispatcher);

    void bindCallbacks();
    void unbindCallbacks();

  public:
    void keyCallback(
        GLFWwindow* window, int key, int scancode, int action, int mods
    );

  private:
    TEventDispatcher* event_dispatcher_;
    GLFWwindow* window_;
};

TInputEngine::TImpl::TImpl(
    GLFWwindow* window, TEventDispatcher* event_dispatcher
)
    : window_(window), event_dispatcher_(event_dispatcher) {
}

///////////////////////////////////////////////////////////////////////////////
// Bind and UnBind methods
///////////////////////////////////////////////////////////////////////////////

void TInputEngine::TImpl::bindCallbacks() {
    glfwSetKeyCallback(window_, KeyCallback);
}

void TInputEngine::TImpl::unbindCallbacks() {
    glfwSetKeyCallback(window_, NULL);
}

////////////////////////////////////////////////////////////////////////////////
// Callback implementations
////////////////////////////////////////////////////////////////////////////////

void TInputEngine::TImpl::keyCallback(
    GLFWwindow* window, int key, int scancode, int action, int mods
) {
    TInputEvent input_event;

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        input_event = TInputEvent{
            .type =
                {
                    .input_device = EInputDevice::KEYBOARD,
                    .key          = EKey::SPACE,
                    .key_action   = EKeyAction::PRESSED,
                },
            .context = {}
        };
    }
    event_dispatcher_->raiseEvent(MakeEvent(input_event));
}

namespace {

static TInputEngine::TImpl* input_engine;

static void KeyCallback(
    GLFWwindow* window, int key, int scancode, int action, int mods
) {
    assert(input_engine);

    input_engine->keyCallback(window, key, scancode, action, mods);
}

}  // namespace

TInputEngine::TInputEngine()
    : impl_(nullptr) {
}
TInputEngine::~TInputEngine() {
}

void TInputEngine::init(
    GLFWwindow* window, TEventDispatcher* event_dispatcher
) {
    assert(!impl_);

    impl_ = std::make_unique<TInputEngine::TImpl>(window, event_dispatcher);
    impl_->bindCallbacks();
    input_engine = impl_.get();
}

void TInputEngine::deinit() {
    assert(!!impl_);

    input_engine = nullptr;
    impl_->unbindCallbacks();
    impl_.reset();
}

}  // namespace NGameEngine
