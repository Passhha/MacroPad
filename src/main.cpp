#include <Arduino.h>
#include <HID.h>

#include "Core/Colors.h"
#include "Core/Config.h"
#include "Core/KeyBindings.h"
#include "Core/Timer.h"
#include "Core/Types.h"
#include "Drivers/Display.h"
#include "Drivers/Encoder.h"
#include "Drivers/Keyboard.h"
#include "Drivers/RGB.h"
#include "Protocol/SerialProtocol.h"
#include "UI/HomeScreen.h"
#include "UI/SettingsScreen.h"

namespace
{
    constexpr uint8_t HidModifierLeftControl = 0x01;
    constexpr uint8_t HidModifierLeftShift = 0x02;
    constexpr uint16_t ConsumerMute = 0x00E2;
    constexpr uint16_t ConsumerVolumeUp = 0x00E9;
    constexpr uint16_t ConsumerVolumeDown = 0x00EA;
    constexpr uint16_t ConsumerPlayPause = 0x00CD;
    constexpr uint16_t ConsumerNextTrack = 0x00B5;
    constexpr uint16_t ConsumerPreviousTrack = 0x00B6;

    const uint8_t HidDescriptor[] PROGMEM =
    {
        0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x85, 0x01,
        0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00,
        0x25, 0x01, 0x75, 0x01, 0x95, 0x08, 0x81, 0x02,
        0x95, 0x01, 0x75, 0x08, 0x81, 0x01, 0x95, 0x06,
        0x75, 0x08, 0x15, 0x00, 0x25, 0x65, 0x05, 0x07,
        0x19, 0x00, 0x29, 0x65, 0x81, 0x00, 0xC0,
        0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x02,
        0x15, 0x00, 0x26, 0xFF, 0x03, 0x19, 0x00, 0x2A,
        0xFF, 0x03, 0x75, 0x10, 0x95, 0x01, 0x81, 0x00,
        0xC0
    };

    struct KeyboardReport
    {
        uint8_t modifiers = 0;
        uint8_t reserved = 0;
        uint8_t keys[6] = {};
    };

    class UsbHid
    {
    public:
        UsbHid()
            : descriptor_(HidDescriptor, sizeof(HidDescriptor))
        {
            HID().AppendDescriptor(&descriptor_);
        }

        void sendKeyboard(const KeyboardReport& report)
        {
            HID().SendReport(1, &report, sizeof(report));
        }

        void sendConsumer(uint16_t usage)
        {
            HID().SendReport(2, &usage, sizeof(usage));
        }

    private:
        HIDSubDescriptor descriptor_;
    };

    DisplayState displayState;
    Display display;
    Encoder encoder;
    Keyboard keyboard;
    KeyBindings bindings;
    RGB rgb;
    HomeScreen homeScreen;
    SettingsScreen settingsScreen;
    SerialProtocol serialProtocol(displayState);
    Timer displayTimer(Config::DisplayPeriod);
    UsbHid usbHid;
    ScreenType activeScreen = ScreenType::Home;
    KeyboardReport previousReport;
    uint16_t consumerUsage = 0;

    bool reportsEqual(const KeyboardReport& left, const KeyboardReport& right)
    {
        if (left.modifiers != right.modifiers)
        {
            return false;
        }

        for (uint8_t index = 0; index < sizeof(left.keys); ++index)
        {
            if (left.keys[index] != right.keys[index])
            {
                return false;
            }
        }

        return true;
    }

    void handleKeyboard()
    {
        const KeyboardState& state = keyboard.state();

        KeyboardReport report;

        for (uint8_t key = 0; key < Config::KeyCount; ++key)
        {
            const KeyBinding& binding = bindings.get(displayState.mode, key);

            if (binding.type == BindingType::ScreenToggle && state.pressed[key])
            {
                activeScreen = activeScreen == ScreenType::Home
                    ? ScreenType::Settings
                    : ScreenType::Home;
                displayState.dirty = true;
                continue;
            }

            if (binding.type == BindingType::Consumer && state.pressed[key])
            {
                consumerUsage = binding.code;
                continue;
            }

            if (binding.type != BindingType::Keyboard ||
                state.keys[key] != KeyState::Pressed)
            {
                continue;
            }

            report.modifiers |= binding.modifiers;

            for (uint8_t slot = 0; slot < sizeof(report.keys); ++slot)
            {
                if (report.keys[slot] == 0)
                {
                    report.keys[slot] = static_cast<uint8_t>(binding.code);
                    break;
                }
            }
        }

        if (!reportsEqual(report, previousReport))
        {
            usbHid.sendKeyboard(report);
            previousReport = report;
        }
    }

    void handleEncoder()
    {
        const EncoderState& state = encoder.state();

        if (state.rotation > 0 && displayState.volume < 100)
        {
            ++displayState.volume;
            consumerUsage = ConsumerVolumeUp;
            displayState.dirty = true;
        }
        else if (state.rotation < 0 && displayState.volume > 0)
        {
            --displayState.volume;
            consumerUsage = ConsumerVolumeDown;
            displayState.dirty = true;
        }

        if (state.click)
        {
            displayState.muted = !displayState.muted;
            consumerUsage = ConsumerMute;
            displayState.dirty = true;
        }
    }

    void updateDisplay()
    {
        if (!displayState.dirty || !displayTimer.elapsed())
        {
            return;
        }

        if (activeScreen == ScreenType::Home)
        {
            homeScreen.draw(display, displayState);
        }
        else
        {
            settingsScreen.draw(display, displayState);
        }

        displayState.dirty = false;
    }
}

void setup()
{
    display.begin();
    encoder.begin();
    keyboard.begin();
    bindings.begin();
    rgb.begin();
    rgb.setColor(0, 16, 32);
    serialProtocol.begin();
    displayState.dirty = true;
}

void loop()
{
    serialProtocol.update();

    BindingCommand command;

    if (serialProtocol.consumeBindingCommand(command))
    {
        if (command.type == BindingCommand::Type::RestoreDefaults)
        {
            bindings.restoreDefaults();
        }
        else if (command.type == BindingCommand::Type::Set)
        {
            bindings.set(command.mode, command.key, command.binding);
        }
    }

    encoder.update();
    keyboard.update();

    consumerUsage = 0;
    handleEncoder();
    handleKeyboard();

    usbHid.sendConsumer(consumerUsage);
    updateDisplay();
    rgb.update();
    display.update();
}
