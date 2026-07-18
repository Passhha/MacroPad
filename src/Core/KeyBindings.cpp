#include "KeyBindings.h"

#include <EEPROM.h>

namespace
{
    constexpr uint16_t StorageMagic = 0x4D50;
    constexpr uint8_t StorageVersion = 1;

    struct StorageHeader
    {
        uint16_t magic;
        uint8_t version;
    };

    constexpr uint16_t BindingsAddress = sizeof(StorageHeader);
    constexpr uint8_t HidModifierLeftControl = 0x01;
    constexpr uint8_t HidModifierLeftShift = 0x02;
    constexpr uint16_t ConsumerPlayPause = 0x00CD;
    constexpr uint16_t ConsumerNextTrack = 0x00B5;
    constexpr uint16_t ConsumerPreviousTrack = 0x00B6;

    KeyBinding makeBinding(BindingType type, uint8_t modifiers, uint16_t code)
    {
        KeyBinding binding;
        binding.type = type;
        binding.modifiers = modifiers;
        binding.code = code;
        return binding;
    }
}

void KeyBindings::begin()
{
    StorageHeader header = {};
    EEPROM.get(0, header);

    if (header.magic != StorageMagic || header.version != StorageVersion)
    {
        restoreDefaults();
        return;
    }

    EEPROM.get(BindingsAddress, bindings_);
}

const KeyBinding& KeyBindings::get(AppMode mode, uint8_t key) const
{
    static const KeyBinding EmptyBinding = {};

    if (key >= Config::KeyCount)
    {
        return EmptyBinding;
    }

    return bindings_[modeIndex(mode)][key];
}

bool KeyBindings::set(AppMode mode, uint8_t key, const KeyBinding& binding)
{
    if (key >= Config::KeyCount || !isValid(binding))
    {
        return false;
    }

    const uint8_t index = modeIndex(mode);
    bindings_[index][key] = binding;
    saveBinding(index, key);
    return true;
}

void KeyBindings::restoreDefaults()
{
    setDefaults();
    saveHeader();
    EEPROM.put(BindingsAddress, bindings_);
}

bool KeyBindings::isValid(const KeyBinding& binding) const
{
    switch (binding.type)
    {
        case BindingType::None:
        case BindingType::ScreenToggle:
            return binding.modifiers == 0 && binding.code == 0;

        case BindingType::Keyboard:
            return binding.code > 0 && binding.code <= 0x65;

        case BindingType::Consumer:
            return binding.code > 0 && binding.code <= 0x03FF;
    }

    return false;
}

uint8_t KeyBindings::modeIndex(AppMode mode) const
{
    return static_cast<uint8_t>(mode);
}

void KeyBindings::setDefaults()
{
    for (uint8_t mode = 0; mode < ModeCount; ++mode)
    {
        for (uint8_t key = 0; key < Config::KeyCount; ++key)
        {
            bindings_[mode][key] = {};
        }

        for (uint8_t key = 0; key < 12; ++key)
        {
            bindings_[mode][key] = makeBinding(BindingType::Keyboard, 0, static_cast<uint16_t>(0x68 + key));
        }

        bindings_[mode][12] = makeBinding(BindingType::Keyboard, HidModifierLeftControl, 0x06);
        bindings_[mode][13] = makeBinding(BindingType::Keyboard, HidModifierLeftControl, 0x19);
        bindings_[mode][14] = makeBinding(BindingType::Keyboard, HidModifierLeftControl, 0x1D);
        bindings_[mode][15] = makeBinding(BindingType::Keyboard, HidModifierLeftControl | HidModifierLeftShift, 0x16);
        bindings_[mode][16] = makeBinding(BindingType::Consumer, 0, ConsumerPlayPause);
        bindings_[mode][17] = makeBinding(BindingType::Consumer, 0, ConsumerNextTrack);
        bindings_[mode][18] = makeBinding(BindingType::Consumer, 0, ConsumerPreviousTrack);
        bindings_[mode][19] = makeBinding(BindingType::ScreenToggle, 0, 0);
    }

    bindings_[modeIndex(AppMode::Chrome)][0] = makeBinding(BindingType::Keyboard, HidModifierLeftControl, 0x1E);
    bindings_[modeIndex(AppMode::Chrome)][1] = makeBinding(BindingType::Keyboard, HidModifierLeftControl, 0x1F);
    bindings_[modeIndex(AppMode::Chrome)][2] = makeBinding(BindingType::Keyboard, HidModifierLeftControl, 0x20);
    bindings_[modeIndex(AppMode::Chrome)][3] = makeBinding(BindingType::Keyboard, HidModifierLeftControl, 0x21);
    bindings_[modeIndex(AppMode::Chrome)][4] = makeBinding(BindingType::Keyboard, HidModifierLeftControl, 0x22);

    bindings_[modeIndex(AppMode::Discord)][0] = makeBinding(BindingType::Keyboard, HidModifierLeftControl | HidModifierLeftShift, 0x10);
    bindings_[modeIndex(AppMode::Discord)][1] = makeBinding(BindingType::Keyboard, HidModifierLeftControl | HidModifierLeftShift, 0x07);
}

void KeyBindings::saveHeader()
{
    const StorageHeader header = {StorageMagic, StorageVersion};
    EEPROM.put(0, header);
}

void KeyBindings::saveBinding(uint8_t mode, uint8_t key)
{
    const uint16_t index = mode * Config::KeyCount + key;
    const uint16_t address = BindingsAddress + index * sizeof(KeyBinding);
    EEPROM.put(address, bindings_[mode][key]);
}
