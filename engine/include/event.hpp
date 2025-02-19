#pragma once

#include <variant>

#include "input_event.hpp"

namespace NGameEngine {

using TEventType = std::variant<TInputEventType>;
using TEvent = std::variant<TInputEvent>;

template <typename TBaseEventType>
TEventType MakeEventType(TBaseEventType event_type) {
    return TEventType{std::forward<TBaseEventType>(event_type)};
}

template <typename TBaseEvent>
TEvent MakeEvent(TBaseEvent event) {
    return TEvent{std::forward<TBaseEvent>(event)};
}

}  // namespace NGameEngine
