#pragma once

#include <cstddef>
namespace NGameEngine {

///////////////////////////////////////////////////
// Input device types
///////////////////////////////////////////////////

enum class EInputDevice : size_t {
    UNKNOWN = 0,
    KEYBOARD,
    MOUSE,
    INPUT_DEVICE_COUNT,
};

///////////////////////////////////////////////////
// Input device keys
///////////////////////////////////////////////////

#define KEY(key) KEY_##key

enum class EKey : size_t {
    UNKNOWN = 0,
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE,
    KEY(A),
    KEY(B),
    KEY(C),
    KEY(D),
    KEY(E),
    KEY(F),
    KEY(G),
    KEY(H),
    KEY(I),
    KEY(J),
    KEY(K),
    KEY(L),
    KEY(M),
    KEY(N),
    KEY(O),
    KEY(P),
    KEY(Q),
    KEY(R),
    KEY(S),
    KEY(T),
    KEY(U),
    KEY(V),
    KEY(W),
    KEY(X),
    KEY(Y),
    KEY(Z),
    KEY(SPACE),
    KEY_COUNT,
};

#undef KEY

///////////////////////////////////////////////////
// Input device key actions
///////////////////////////////////////////////////

enum class EKeyAction : size_t {
    UNKNOWN = 0,
    PRESSED,
    RELEASED,
    MOVED,
    KEY_ACTION_COUNT,
};

///////////////////////////////////////////////////
// Input events
///////////////////////////////////////////////////

struct TInputEventType {
    EInputDevice input_device = EInputDevice::UNKNOWN;
    EKey key                  = EKey::UNKNOWN;
    EKeyAction key_action     = EKeyAction::UNKNOWN;
};

struct TKeyboardEventContext {};

struct TMouseEventContext {
    int prev_x;
    int prev_y;
    int curr_x;
    int curr_y;
};

struct TInputEventContext {
    union {
        TMouseEventContext mouse;
        TKeyboardEventContext keyboard;
    };
};

struct TInputEvent {
    TInputEventType type;
    TInputEventContext context;
};

}  // namespace NGameEngine
