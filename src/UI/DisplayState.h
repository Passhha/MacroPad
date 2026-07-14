#pragma once

#include <stdint.h>

#include "../Core/Types.h"

struct DisplayState
{
    // Что сейчас регулирует энкодер
    AppMode mode = AppMode::System;

    // Громкость 0..100
    uint8_t volume = 50;

    // Состояние звука
    bool muted = false;

    // Требуется ли обновить экран
    bool dirty = true;
};