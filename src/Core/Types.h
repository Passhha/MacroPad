#pragma once

#include <stdint.h>

#include "Config.h"

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

struct KeyboardState
{
    KeyState keys[Config::KeyCount] = {};

    bool pressed[Config::KeyCount] = {};

    bool released[Config::KeyCount] = {};
};

enum class BindingType : uint8_t
{
    None = 0,
    Keyboard,
    Consumer,
    ScreenToggle
};

struct KeyBinding
{
    BindingType type = BindingType::None;

    uint8_t modifiers = 0;

    uint16_t code = 0;
};
