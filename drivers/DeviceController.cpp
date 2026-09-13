#include <Arduino.h>

#include "../config/Pins.h"

#include "DeviceController.h"


#define DEVICE_ON HIGH
#define DEVICE_OFF LOW


void deviceOn(int pin) {

    digitalWrite(pin, DEVICE_ON);
}


void deviceOff(int pin) {

    digitalWrite(pin, DEVICE_OFF);
}


void setupDevices() {

    pinMode(SAMPLE_PUMP, OUTPUT);

    pinMode(NO2_PUMP_1, OUTPUT);

    pinMode(NO2_PUMP_2, OUTPUT);

    pinMode(DRAIN_VALVE, OUTPUT);


    allDevicesOff();
}


void allDevicesOff() {

    deviceOff(SAMPLE_PUMP);

    deviceOff(NO2_PUMP_1);

    deviceOff(NO2_PUMP_2);

    deviceOff(DRAIN_VALVE);
}