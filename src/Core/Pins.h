#pragma once

#include <Arduino.h>

namespace Pins
{
    // ===========================
    // Encoder
    // ===========================

    constexpr uint8_t EncoderA = 2;
    constexpr uint8_t EncoderB = 3;
    constexpr uint8_t EncoderButton = 4;

    // ===========================
    // Keyboard Matrix
    // ===========================

    constexpr uint8_t Row0 = 5;
    constexpr uint8_t Row1 = 6;
    constexpr uint8_t Row2 = 7;
    constexpr uint8_t Row3 = 8;

    constexpr uint8_t Col0 = 9;
    constexpr uint8_t Col1 = 10;
    constexpr uint8_t Col2 = 16;
    constexpr uint8_t Col3 = 14;
    constexpr uint8_t Col4 = 15;

    // ===========================
    // TFT Display
    // ===========================

    constexpr uint8_t TFT_DC = A3;
    constexpr uint8_t TFT_RST = A2;

    // TFT_CS использует D10 совместно с аппаратным SPI.
    // Если этот вывод останется занятым дисплеем, матрицу
    // нужно будет перенести на другой свободный пин.
    constexpr uint8_t TFT_CS = 10;

    // ===========================
    // RGB
    // ===========================

    constexpr uint8_t RGB = A1;
}