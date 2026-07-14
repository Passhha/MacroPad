#pragma once

#include "DisplayState.h"

class Display;

class HomeScreen
{
public:

    void draw(
        Display& display,
        const DisplayState& state);

private:

    void drawHeader(
        Display& display,
        const DisplayState& state);

    void drawVolume(
        Display& display,
        const DisplayState& state);

    void drawMute(
        Display& display,
        const DisplayState& state);
};