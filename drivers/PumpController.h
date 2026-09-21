#ifndef PUMP_CONTROLLER_H
#define PUMP_CONTROLLER_H

#include <Arduino.h>

void setupPumps();

void pumpOn(uint8_t pin);

void pumpOff(uint8_t pin);

void stopAllPumps();

void runPumpFor(
    uint8_t pin,
    unsigned long durationMs
);

void doseDrops(
    uint8_t pin,
    int drops,
    unsigned long dropTimeMs
);

#endif