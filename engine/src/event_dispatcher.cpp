#include "event_dispatcher.hpp"

namespace NGameEngine {

class TEventDispatcher::TImpl {
  public:
    TImpl() = default;

    void raiseEvent(TEvent event);
    void registerEventHandler(TEventType event, TEventHandler handler);

  private:
    std::array<
        std::array<
            std::array<
                TEventHandler,
                static_cast<size_t>(EKeyAction::KEY_ACTION_COUNT)>,
            static_cast<size_t>(EKey::KEY_COUNT)>,
        static_cast<size_t>(EInputDevice::INPUT_DEVICE_COUNT)>
        input_handlers_;
};

void TEventDispatcher::TImpl::raiseEvent(TEvent event) {
    if (const auto* input_event = std::get_if<TInputEvent>(&event);
        input_event) {
        auto& handler =
            input_handlers_[static_cast<size_t>(input_event->type.input_device)]
                           [static_cast<size_t>(input_event->type.key)]
                           [static_cast<size_t>(input_event->type.key_action)];
        if (handler) {
            handler(std::move(event));
        }
    }
}

void TEventDispatcher::TImpl::registerEventHandler(
    TEventType event_type, TEventHandler handler
) {
    if (const auto* input_event = std::get_if<TInputEventType>(&event_type);
        input_event) {
        auto& h =
            input_handlers_[static_cast<size_t>(input_event->input_device)]
                           [static_cast<size_t>(input_event->key)]
                           [static_cast<size_t>(input_event->key_action)];
        h = std::move(handler);
    }
}

TEventDispatcher::TEventDispatcher()
    : impl_(std::make_unique<TImpl>()) {
}

TEventDispatcher::~TEventDispatcher() {
}

void TEventDispatcher::raiseEvent(TEvent event) {
    impl_->raiseEvent(std::move(event));
}

void TEventDispatcher::registerEventHandler(
    TEventType event_type, TEventHandler handler
) {
    impl_->registerEventHandler(std::move(event_type), std::move(handler));
}

}  // namespace NGameEngine
