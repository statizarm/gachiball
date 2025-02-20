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

enum class EKey : size_t {
    UNKNOWN = 0,
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE,
    SPACE,
    KEY_COUNT,
};

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
