#pragma once

#include <stdint.h>

class Display
{
public:

    bool begin();

    void update();

    void clear(uint16_t color = 0x0000);

    void drawPixel(
        int16_t x,
        int16_t y,
        uint16_t color);

    void drawLine(
        int16_t x0,
        int16_t y0,
        int16_t x1,
        int16_t y1,
        uint16_t color);

    void drawCircle(
        int16_t x,
        int16_t y,
        int16_t radius,
        uint16_t color);

    void fillCircle(
        int16_t x,
        int16_t y,
        int16_t radius,
        uint16_t color);

    void drawRect(
        int16_t x,
        int16_t y,
        int16_t width,
        int16_t height,
        uint16_t color);

    void fillRect(
        int16_t x,
        int16_t y,
        int16_t width,
        int16_t height,
        uint16_t color);

    void drawText(
        int16_t x,
        int16_t y,
        const char* text,
        uint16_t color,
        uint8_t size = 1);

    void drawCenteredText(
        const char* text,
        int16_t y,
        uint16_t color,
        uint8_t size = 1);

    uint16_t width() const;

    uint16_t height() const;

private:

    bool initialized_ = false;
};