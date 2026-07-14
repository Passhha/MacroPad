#pragma once

#include <Arduino.h>

class Keyboard
{
public:

    void begin();

    void update();

    bool pressed(uint8_t key);

private:

    bool state[2] = {false, false};
};