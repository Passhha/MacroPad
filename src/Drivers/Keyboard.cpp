#include "Keyboard.h"
#include "../Core/Pins.h"

void Keyboard::begin()
{
    pinMode(ROW0, OUTPUT);

    digitalWrite(ROW0, LOW);

    pinMode(COL0, INPUT_PULLUP);
    pinMode(COL1, INPUT_PULLUP);
}

void Keyboard::update()
{
    state[0] = digitalRead(COL0) == LOW;
    state[1] = digitalRead(COL1) == LOW;
}

bool Keyboard::pressed(uint8_t key)
{
    return state[key];
}