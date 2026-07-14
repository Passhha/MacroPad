#include <Arduino.h>

#include "App/Application.h"

Application app;

void setup()
{
    app.begin();
}

void loop()
{
    app.update();
}