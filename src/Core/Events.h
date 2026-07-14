#pragma once

#include <stdint.h>

enum class EventType : uint8_t
{
    None,

    EncoderClockwise,
    EncoderCounterClockwise,

    EncoderPressed,

    KeyPressed,
    KeyReleased,

    UsbConnected,
    UsbDisconnected
};

struct Event
{
    EventType type;

    uint8_t data = 0;
};