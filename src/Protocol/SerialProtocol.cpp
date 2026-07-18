#include "SerialProtocol.h"

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

namespace
{
    bool parseNumber(char* text, long& value)
    {
        char* end = nullptr;
        value = strtol(text, &end, 10);
        return end != text && *end == '\0';
    }

    bool parseMode(long value, AppMode& mode)
    {
        if (value < 0 || value > 2)
        {
            return false;
        }

        mode = static_cast<AppMode>(value);
        return true;
    }
}

SerialProtocol::SerialProtocol(DisplayState& state)
    : state_(state)
{
}

void SerialProtocol::begin()
{
    Serial.begin(115200);
}

void SerialProtocol::update()
{
    while (Serial.available() > 0)
    {
        const char character = static_cast<char>(Serial.read());

        if (character == '\n' || character == '\r')
        {
            if (length_ > 0)
            {
                buffer_[length_] = '\0';
                processCommand();
                length_ = 0;
            }
        }
        else if (length_ < sizeof(buffer_) - 1)
        {
            buffer_[length_++] = character;
        }
        else
        {
            length_ = 0;
        }
    }
}

bool SerialProtocol::consumeBindingCommand(BindingCommand& command)
{
    if (pendingCommand_.type == BindingCommand::Type::None)
    {
        return false;
    }

    command = pendingCommand_;
    pendingCommand_ = {};
    return true;
}

void SerialProtocol::processCommand()
{
    char* context = nullptr;
    char* command = strtok_r(buffer_, " ", &context);

    if (command == nullptr)
    {
        return;
    }

    if (strcmp(command, "STATE") == 0)
    {
        printState();
        return;
    }

    if (strcmp(command, "DEFAULTS") == 0)
    {
        pendingCommand_.type = BindingCommand::Type::RestoreDefaults;
        Serial.println("OK");
        return;
    }

    if (strcmp(command, "BIND") == 0)
    {
        if (parseBindingCommand(command, context))
        {
            Serial.println("OK");
        }
        else
        {
            Serial.println("ERR");
        }

        return;
    }

    char* argument = strtok_r(nullptr, " ", &context);

    if (strcmp(command, "VOL") == 0 && argument != nullptr)
    {
        const long volume = strtol(argument, nullptr, 10);

        if (volume >= 0 && volume <= 100)
        {
            state_.volume = static_cast<uint8_t>(volume);
            state_.dirty = true;
        }

        return;
    }

    if (strcmp(command, "MUTE") == 0 && argument != nullptr)
    {
        state_.muted = argument[0] == '1';
        state_.dirty = true;
        return;
    }

    if (strcmp(command, "MODE") == 0 && argument != nullptr && strcmp(argument, "SYSTEM") == 0)
    {
        state_.mode = AppMode::System;
    }
    else if (strcmp(command, "MODE") == 0 && argument != nullptr && strcmp(argument, "CHROME") == 0)
    {
        state_.mode = AppMode::Chrome;
    }
    else if (strcmp(command, "MODE") == 0 && argument != nullptr && strcmp(argument, "DISCORD") == 0)
    {
        state_.mode = AppMode::Discord;
    }
    else
    {
        Serial.println("ERR");
        return;
    }

    state_.dirty = true;
}

bool SerialProtocol::parseBindingCommand(char* command, char* arguments)
{
    (void)command;

    char* context = nullptr;
    char* modeText = strtok_r(arguments, " ", &context);
    char* keyText = strtok_r(nullptr, " ", &context);
    char* typeText = strtok_r(nullptr, " ", &context);
    char* modifiersText = strtok_r(nullptr, " ", &context);
    char* codeText = strtok_r(nullptr, " ", &context);

    if (modeText == nullptr || keyText == nullptr || typeText == nullptr ||
        modifiersText == nullptr || codeText == nullptr)
    {
        return false;
    }

    long modeValue;
    long keyValue;
    long typeValue;
    long modifiersValue;
    long codeValue;

    if (!parseNumber(modeText, modeValue) || !parseNumber(keyText, keyValue) ||
        !parseNumber(typeText, typeValue) || !parseNumber(modifiersText, modifiersValue) ||
        !parseNumber(codeText, codeValue) || keyValue < 0 ||
        keyValue >= Config::KeyCount || typeValue < 0 || typeValue > 3 ||
        modifiersValue < 0 || modifiersValue > 255 || codeValue < 0 || codeValue > 1023)
    {
        return false;
    }

    AppMode mode;

    if (!parseMode(modeValue, mode))
    {
        return false;
    }

    pendingCommand_.type = BindingCommand::Type::Set;
    pendingCommand_.mode = mode;
    pendingCommand_.key = static_cast<uint8_t>(keyValue);
    pendingCommand_.binding.type = static_cast<BindingType>(typeValue);
    pendingCommand_.binding.modifiers = static_cast<uint8_t>(modifiersValue);
    pendingCommand_.binding.code = static_cast<uint16_t>(codeValue);
    return true;
}

void SerialProtocol::printState() const
{
    const char* mode = state_.mode == AppMode::System ? "SYSTEM"
        : state_.mode == AppMode::Chrome ? "CHROME"
        : "DISCORD";

    Serial.print("MODE ");
    Serial.print(mode);
    Serial.print(" VOL ");
    Serial.print(state_.volume);
    Serial.print(" MUTE ");
    Serial.println(state_.muted ? 1 : 0);
}
