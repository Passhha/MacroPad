#pragma once

#include <Arduino.h>

class Timer
{
public:

    Timer() = default;

    explicit Timer(uint32_t interval);

    void setInterval(uint32_t interval);

    void restart();

    bool elapsed();

    bool expired() const;

private:

    uint32_t interval_ = 0;

    uint32_t previous_ = 0;

    bool expired_ = false;
};