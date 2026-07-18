#pragma once

#include <Arduino.h>

#include "../Core/Config.h"
#include "../Core/Types.h"

class Keyboard
{
public:

    void begin();

    void update();

    const KeyboardState& state() const;

private:

    bool rawState_[Config::KeyCount] = {};

    uint32_t rawChangedAt_[Config::KeyCount] = {};

    KeyboardState state_;
};
