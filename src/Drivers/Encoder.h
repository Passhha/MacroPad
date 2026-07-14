#pragma once

#include "../Core/Types.h"

class Encoder
{
public:

    void begin();

    void update();

    const EncoderState& state() const;

private:

    uint8_t previousState_ = 0;

    bool previousButton_ = false;

    int8_t accumulator_ = 0;

    EncoderState state_;
};