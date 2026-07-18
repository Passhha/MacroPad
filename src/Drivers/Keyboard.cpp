#include "Keyboard.h"

#include "../Core/Pins.h"

namespace
{
    constexpr uint8_t Rows[Config::MatrixRows] =
    {
        Pins::Row0,
        Pins::Row1,
        Pins::Row2,
        Pins::Row3
    };

    constexpr uint8_t Columns[Config::MatrixColumns] =
    {
        Pins::Col0,
        Pins::Col1,
        Pins::Col2,
        Pins::Col3,
        Pins::Col4
    };
}

void Keyboard::begin()
{
    for (uint8_t row = 0; row < Config::MatrixRows; ++row)
    {
        pinMode(Rows[row], OUTPUT);
        digitalWrite(Rows[row], HIGH);
    }

    for (uint8_t column = 0; column < Config::MatrixColumns; ++column)
    {
        pinMode(Columns[column], INPUT_PULLUP);
    }

    state_ = {};

    const uint32_t now = millis();

    for (uint8_t key = 0; key < Config::KeyCount; ++key)
    {
        rawChangedAt_[key] = now;
    }
}

void Keyboard::update()
{
    for (uint8_t key = 0; key < Config::KeyCount; ++key)
    {
        state_.pressed[key] = false;
        state_.released[key] = false;
    }

    for (uint8_t row = 0; row < Config::MatrixRows; ++row)
    {
        digitalWrite(Rows[row], LOW);

        for (uint8_t column = 0; column < Config::MatrixColumns; ++column)
        {
            const uint8_t key = row * Config::MatrixColumns + column;
            const bool isPressed = digitalRead(Columns[column]) == LOW;

            if (isPressed != rawState_[key])
            {
                rawState_[key] = isPressed;
                rawChangedAt_[key] = millis();
            }

            if (millis() - rawChangedAt_[key] >= Config::KeyboardDebounceMs)
            {
                const KeyState newState = isPressed
                    ? KeyState::Pressed
                    : KeyState::Released;

                if (state_.keys[key] != newState)
                {
                    state_.pressed[key] = newState == KeyState::Pressed;
                    state_.released[key] = newState == KeyState::Released;
                    state_.keys[key] = newState;
                }
            }
        }

        digitalWrite(Rows[row], HIGH);
    }
}

const KeyboardState& Keyboard::state() const
{
    return state_;
}
