#include "RGB.h"

#include "../Core/Config.h"
#include "../Core/Pins.h"

#include <Arduino.h>
#include <avr/interrupt.h>

namespace
{
    void writeByte(volatile uint8_t* port, uint8_t mask, uint8_t value)
    {
        for (uint8_t bit = 0x80; bit != 0; bit >>= 1)
        {
            *port |= mask;

            if (value & bit)
            {
                asm volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\t");
            }

            *port &= static_cast<uint8_t>(~mask);
            asm volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\t");
        }
    }
}

void RGB::begin()
{
    pinMode(Pins::RGB, OUTPUT);
    digitalWrite(Pins::RGB, LOW);
    write();
}

void RGB::update()
{
    if (dirty_)
    {
        write();
    }
}

void RGB::setBrightness(uint8_t brightness)
{
    if (brightness_ != brightness)
    {
        brightness_ = brightness;
        dirty_ = true;
    }
}

void RGB::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    if (red_ != red || green_ != green || blue_ != blue)
    {
        red_ = red;
        green_ = green;
        blue_ = blue;
        dirty_ = true;
    }
}

void RGB::write()
{
    const uint8_t port = digitalPinToPort(Pins::RGB);
    const uint8_t mask = digitalPinToBitMask(Pins::RGB);
    volatile uint8_t* output = portOutputRegister(port);
    const uint8_t savedStatus = SREG;

    const uint8_t red = (static_cast<uint16_t>(red_) * brightness_) / Config::MaxBrightness;
    const uint8_t green = (static_cast<uint16_t>(green_) * brightness_) / Config::MaxBrightness;
    const uint8_t blue = (static_cast<uint16_t>(blue_) * brightness_) / Config::MaxBrightness;

    noInterrupts();
    writeByte(output, mask, green);
    writeByte(output, mask, red);
    writeByte(output, mask, blue);
    *output &= static_cast<uint8_t>(~mask);
    SREG = savedStatus;

    delayMicroseconds(80);
    dirty_ = false;
}
