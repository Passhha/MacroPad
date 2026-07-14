#include "Encoder.h"

#include "../Core/Config.h"
#include "../Core/Pins.h"

#include <Arduino.h>

namespace
{
    constexpr int8_t TransitionTable[16] =
    {
         0, -1,  1,  0,
         1,  0,  0, -1,
        -1,  0,  0,  1,
         0,  1, -1,  0
    };
}

void Encoder::begin()
{
    pinMode(Pins::EncoderA, INPUT_PULLUP);
    pinMode(Pins::EncoderB, INPUT_PULLUP);
    pinMode(Pins::EncoderButton, INPUT_PULLUP);

    previousState_ =
        (digitalRead(Pins::EncoderA) << 1) |
         digitalRead(Pins::EncoderB);

    previousButton_ =
        digitalRead(Pins::EncoderButton) == LOW;

    accumulator_ = 0;

    state_ = {};
}

void Encoder::update()
{
    state_.rotation = 0;
    state_.click = false;

    uint8_t currentState =
        (digitalRead(Pins::EncoderA) << 1) |
         digitalRead(Pins::EncoderB);

    uint8_t index =
        (previousState_ << 2) |
         currentState;

    accumulator_ += TransitionTable[index];

    previousState_ = currentState;

    if (accumulator_ >= Config::EncoderSteps)
    {
        accumulator_ = 0;
        state_.rotation = 1;
    }

    if (accumulator_ <= -Config::EncoderSteps)
    {
        accumulator_ = 0;
        state_.rotation = -1;
    }

    bool button =
        digitalRead(Pins::EncoderButton) == LOW;

    state_.pressed = button;

    if (!previousButton_ && button)
    {
        state_.click = true;
    }

    previousButton_ = button;
}

const EncoderState& Encoder::state() const
{
    return state_;
}