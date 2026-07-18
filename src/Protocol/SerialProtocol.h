#pragma once

#include "../UI/DisplayState.h"
#include "../Core/Types.h"

struct BindingCommand
{
    enum class Type : uint8_t
    {
        None,
        Set,
        RestoreDefaults
    };

    Type type = Type::None;
    AppMode mode = AppMode::System;
    uint8_t key = 0;
    KeyBinding binding;
};

class SerialProtocol
{
public:
    explicit SerialProtocol(DisplayState& state);

    void begin();

    void update();

    bool consumeBindingCommand(BindingCommand& command);

private:
    void processCommand();
    void printState() const;
    bool parseBindingCommand(char* command, char* arguments);

    DisplayState& state_;
    char buffer_[32] = {};
    uint8_t length_ = 0;
    BindingCommand pendingCommand_;
};
