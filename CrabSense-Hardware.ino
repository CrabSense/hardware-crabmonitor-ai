#include "config/Pins.h"
#include "drivers/DeviceController.h"
#include "modules/NO2/NO2Test.h"

void setup() {

    Serial.begin(115200);

    setupDevices();

    Serial.println("CrabSense Hardware Started");
}

void loop() {

    runNO2Test();

    delay(300000);
}