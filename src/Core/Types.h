#pragma once

#include <stdint.h>

enum class AppMode : uint8_t
{
    System = 0,

    Chrome,

    Discord
};

enum class ScreenType : uint8_t
{
    Home = 0,

    Settings
};

struct EncoderState
{
    int8_t rotation = 0;

    bool click = false;

    bool pressed = false;
};

enum class KeyState : uint8_t
{
    Released = 0,

    Pressed
};