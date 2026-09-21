#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include <Arduino.h>

void setupRelay();

void openDrainValve();

void closeDrainValve();

void drainFor(unsigned long durationMs);

#endif