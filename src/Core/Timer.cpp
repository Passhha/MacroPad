#include "Timer.h"

Timer::Timer(uint32_t interval)
{
    interval_ = interval;

    previous_ = millis();
}

void Timer::setInterval(uint32_t interval)
{
    interval_ = interval;
}

void Timer::restart()
{
    previous_ = millis();

    expired_ = false;
}

bool Timer::elapsed()
{
    const uint32_t now = millis();

    if (now - previous_ >= interval_)
    {
        previous_ = now;

        expired_ = true;

        return true;
    }

    expired_ = false;

    return false;
}

bool Timer::expired() const
{
    return expired_;
}