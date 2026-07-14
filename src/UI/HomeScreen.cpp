#include <stdio.h>
#include "HomeScreen.h"
#include "../Drivers/Display.h"
#include "../Core/Colors.h"

void HomeScreen::draw(
    Display& display,
    const DisplayState& state)
{
    display.clear();

    drawHeader(display, state);

    drawVolume(display, state);

    drawMute(display, state);
}

void HomeScreen::drawHeader(
    Display& display,
    const DisplayState& state)
{
    const char* mode = "";

    switch (state.mode)
    {
        case AppMode::System:
            mode = "SYSTEM";
            break;

        case AppMode::Chrome:
            mode = "CHROME";
            break;

        case AppMode::Discord:
            mode = "DISCORD";
            break;
    }

    display.drawCenteredText(
        mode,
        20,
        Colors::White,
        2);
}

void HomeScreen::drawVolume(
    Display& display,
    const DisplayState& state)
{
    char buffer[8];

    snprintf(
        buffer,
        sizeof(buffer),
        "%u%%",
        state.volume);

    display.drawCenteredText(
        buffer,
        105,
        Colors::White,
        4);
}

void HomeScreen::drawMute(
    Display& display,
    const DisplayState& state)
{
    if (state.muted)
    {
        display.drawCenteredText(
            "MUTED",
            185,
            Colors::Red,
            2);
    }
    else
    {
        display.drawCenteredText(
            "ACTIVE",
            185,
            Colors::Green,
            2);
    }
}