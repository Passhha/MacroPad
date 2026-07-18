#pragma once

#include <stdint.h>

class RGB
{
public:
    void begin();

    void update();

    void setBrightness(uint8_t brightness);

    void setColor(uint8_t red, uint8_t green, uint8_t blue);

private:
    void write();

    uint8_t red_ = 0;
    uint8_t green_ = 0;
    uint8_t blue_ = 0;
    uint8_t brightness_ = 32;
    bool dirty_ = true;
};
