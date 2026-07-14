#include "Display.h"

#include "../Core/Config.h"
#include "../Core/Pins.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>

namespace
{
    Adafruit_GC9A01A lcd(
        Pins::TFT_CS,
        Pins::TFT_DC,
        Pins::TFT_RST);
}

bool Display::begin()
{
    lcd.begin();

    lcd.setRotation(0);

    lcd.fillScreen(0x0000);

    initialized_ = true;

    return true;
}

void Display::update()
{
    // Пока пусто.
    // Позже здесь появится управление яркостью,
    // синхронизация кадров и другие сервисные задачи.
}

void Display::clear(uint16_t color)
{
    lcd.fillScreen(color);
}

void Display::drawPixel(
    int16_t x,
    int16_t y,
    uint16_t color)
{
    lcd.drawPixel(x, y, color);
}

void Display::drawLine(
    int16_t x0,
    int16_t y0,
    int16_t x1,
    int16_t y1,
    uint16_t color)
{
    lcd.drawLine(x0, y0, x1, y1, color);
}

void Display::drawCircle(
    int16_t x,
    int16_t y,
    int16_t radius,
    uint16_t color)
{
    lcd.drawCircle(x, y, radius, color);
}

void Display::fillCircle(
    int16_t x,
    int16_t y,
    int16_t radius,
    uint16_t color)
{
    lcd.fillCircle(x, y, radius, color);
}

void Display::drawRect(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    uint16_t color)
{
    lcd.drawRect(x, y, width, height, color);
}

void Display::fillRect(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    uint16_t color)
{
    lcd.fillRect(x, y, width, height, color);
}

void Display::drawText(
    int16_t x,
    int16_t y,
    const char* text,
    uint16_t color,
    uint8_t size)
{
    lcd.setTextSize(size);
    lcd.setTextColor(color);
    lcd.setCursor(x, y);
    lcd.print(text);
}

void Display::drawCenteredText(
    const char* text,
    int16_t y,
    uint16_t color,
    uint8_t size)
{
    lcd.setTextSize(size);
    lcd.setTextColor(color);

    int16_t x1;
    int16_t y1;

    uint16_t w;
    uint16_t h;

    lcd.getTextBounds(
        text,
        0,
        0,
        &x1,
        &y1,
        &w,
        &h);

    lcd.setCursor(
        (Config::ScreenWidth - w) / 2,
        y);

    lcd.print(text);
}

uint16_t Display::width() const
{
    return Config::ScreenWidth;
}

uint16_t Display::height() const
{
    return Config::ScreenHeight;
}