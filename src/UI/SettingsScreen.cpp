#include "SettingsScreen.h"

#include "../Core/Colors.h"
#include "../Drivers/Display.h"

void SettingsScreen::draw(Display& display, const DisplayState& state)
{
    display.clear();
    display.drawCenteredText("SETTINGS", 24, Colors::White, 2);
    display.drawCenteredText("USB HID READY", 82, Colors::Green, 1);
    display.drawCenteredText("KEY 20: HOME", 122, Colors::Gray, 1);

    const char* mode = state.mode == AppMode::System ? "SYSTEM"
        : state.mode == AppMode::Chrome ? "CHROME"
        : "DISCORD";

    display.drawCenteredText(mode, 174, Colors::Cyan, 2);
}
