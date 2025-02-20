#include "input_engine.hpp"

#include <GLFW/glfw3.h>

#include <cassert>

namespace NGameEngine {

namespace {

static void KeyCallback(void*, int key, int scancode, int action, int mods);
static void MouseKeyCallback(void*, int key, int action, int mods);
static void CursorPositionCallback(void*, double xpos, double ypos);

}  // namespace

class TInputEngine::TImpl {
  public:
    TImpl(TWindow* window, TEventDispatcher* event_dispatcher);

    void init();
    void deinit();

  public:
    void keyCallback(int key, int scancode, int action, int mods);
    void cursorPositionCallback(double xpos, double ypos);
    void mouseKeyCallback(int key, int action, int mods);

  private:
    TEventDispatcher* event_dispatcher_;
    TWindow* window_;
};

TInputEngine::TImpl::TImpl(TWindow* window, TEventDispatcher* event_dispatcher)
    : window_(window), event_dispatcher_(event_dispatcher) {
}

///////////////////////////////////////////////////////////////////////////////
// Bind and UnBind methods
///////////////////////////////////////////////////////////////////////////////

void TInputEngine::TImpl::init() {
    window_->registerKeyboardKeyCallback(KeyCallback);
    window_->registerMouseKeyCallback(MouseKeyCallback);
    window_->registerCursorPositionCallback(CursorPositionCallback);
}

void TInputEngine::TImpl::deinit() {
    window_->registerKeyboardKeyCallback(NULL);
    window_->registerMouseKeyCallback(NULL);
    window_->registerCursorPositionCallback(NULL);
}

////////////////////////////////////////////////////////////////////////////////
// Translate Helpers
////////////////////////////////////////////////////////////////////////////////

EKey TranslateGLFWKey(int key) {
#define CASE_KEY(k)    \
    case GLFW_KEY_##k: \
        return EKey::KEY_##k
#define CASE_MOUSE_KEY(key)       \
    case GLFW_MOUSE_BUTTON_##key: \
        return EKey::MOUSE_##key

    switch (key) {
        CASE_KEY(A);
        CASE_KEY(B);
        CASE_KEY(C);
        CASE_KEY(D);
        CASE_KEY(E);
        CASE_KEY(F);
        CASE_KEY(G);
        CASE_KEY(H);
        CASE_KEY(I);
        CASE_KEY(J);
        CASE_KEY(K);
        CASE_KEY(L);
        CASE_KEY(M);
        CASE_KEY(N);
        CASE_KEY(O);
        CASE_KEY(P);
        CASE_KEY(Q);
        CASE_KEY(R);
        CASE_KEY(S);
        CASE_KEY(T);
        CASE_KEY(U);
        CASE_KEY(V);
        CASE_KEY(W);
        CASE_KEY(X);
        CASE_KEY(Y);
        CASE_KEY(Z);
        CASE_KEY(SPACE);
        CASE_MOUSE_KEY(LEFT);
        CASE_MOUSE_KEY(RIGHT);
        CASE_MOUSE_KEY(MIDDLE);
        default:
            return EKey::UNKNOWN;
    }

#undef CASE_MOUSE_KEY
#undef CASE_KEY
}

EKeyAction TranslateGLFWKeyAction(int action) {
    switch (action) {
        case GLFW_PRESS:
            return EKeyAction::PRESSED;
        case GLFW_RELEASE:
            return EKeyAction::RELEASED;
        default:
            return EKeyAction::UNKNOWN;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Callback implementations
////////////////////////////////////////////////////////////////////////////////

void TInputEngine::TImpl::keyCallback(
    int key, int scancode, int action, int mods
) {
    auto input_event = TInputEvent{
        .type =
            {
                .input_device = EInputDevice::KEYBOARD,
                .key          = TranslateGLFWKey(key),
                .key_action   = TranslateGLFWKeyAction(action),
            },
        .context = {}
    };
    event_dispatcher_->raiseEvent(MakeEvent(input_event));
}

void TInputEngine::TImpl::mouseKeyCallback(int key, int action, int mods) {
    auto input_event = TInputEvent{
        .type =
            {
                .input_device = EInputDevice::MOUSE,
                .key          = TranslateGLFWKey(key),
                .key_action   = TranslateGLFWKeyAction(action),
            },
        .context = {}
    };
    event_dispatcher_->raiseEvent(MakeEvent(std::move(input_event)));
}

void TInputEngine::TImpl::cursorPositionCallback(double xpos, double ypos) {
    auto input_event = TInputEvent{
        .type =
            {
                .input_device = EInputDevice::MOUSE,
                .key          = EKey::MOUSE,
                .key_action   = EKeyAction::MOVED,
            },
    };
    input_event.context.mouse = {
        .xpos = xpos,
        .ypos = ypos,
    };
    event_dispatcher_->raiseEvent(MakeEvent(std::move(input_event)));
}

namespace {
static TInputEngine::TImpl* input_engine;

static void KeyCallback(void*, int key, int scancode, int action, int mods) {
    assert(input_engine);

    input_engine->keyCallback(key, scancode, action, mods);
}

static void MouseKeyCallback(void*, int key, int action, int mods) {
    assert(input_engine);

    input_engine->mouseKeyCallback(key, action, mods);
}

static void CursorPositionCallback(void*, double xpos, double ypos) {
    assert(input_engine);

    input_engine->cursorPositionCallback(xpos, ypos);
}

}  // namespace

TInputEngine::TInputEngine()
    : impl_(nullptr) {
}
TInputEngine::~TInputEngine() {
}

void TInputEngine::init(TWindow* window, TEventDispatcher* event_dispatcher) {
    assert(!impl_);

    impl_ = std::make_unique<TInputEngine::TImpl>(window, event_dispatcher);
    impl_->init();
    input_engine = impl_.get();
}

void TInputEngine::deinit() {
    assert(!!impl_);

    input_engine = nullptr;
    impl_->deinit();
    impl_.reset();
}

}  // namespace NGameEngine
