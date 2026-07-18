#pragma once

#include "Types.h"

class KeyBindings
{
public:
    void begin();

    const KeyBinding& get(AppMode mode, uint8_t key) const;

    bool set(AppMode mode, uint8_t key, const KeyBinding& binding);

    void restoreDefaults();

private:
    static constexpr uint8_t ModeCount = 3;

    bool isValid(const KeyBinding& binding) const;
    uint8_t modeIndex(AppMode mode) const;
    void setDefaults();
    void saveHeader();
    void saveBinding(uint8_t mode, uint8_t key);

    KeyBinding bindings_[ModeCount][Config::KeyCount] = {};
};
