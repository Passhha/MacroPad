#pragma once

#include <stdint.h>

namespace Config
{
    // ===========================
    // Display
    // ===========================

    constexpr uint16_t ScreenWidth = 240;
    constexpr uint16_t ScreenHeight = 240;

    constexpr uint8_t DisplayFPS = 30;

    // ===========================
    // Keyboard
    // ===========================

    constexpr uint8_t MatrixRows = 4;
    constexpr uint8_t MatrixColumns = 5;

    constexpr uint8_t KeyCount =
        MatrixRows * MatrixColumns;

    // ===========================
    // Encoder
    // ===========================

    constexpr uint8_t EncoderSteps = 4;

    // ===========================
    // RGB
    // ===========================

    constexpr uint8_t MaxBrightness = 255;

    // ===========================
    // Timing
    // ===========================

    constexpr uint16_t DisplayPeriod =
        1000 / DisplayFPS;
}