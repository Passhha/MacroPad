#pragma once

#include "DisplayState.h"

class Display;

class SettingsScreen
{
public:
    void draw(Display& display, const DisplayState& state);
};
