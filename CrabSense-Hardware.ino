#include <Arduino.h>

#include "config/Pins.h"
#include "config/Settings.h"
#include "drivers/PumpController.h"
#include "drivers/RelayController.h"
#include "modules/NO2/NO2Test.h"


void printMenu()
{
    Serial.println();
    Serial.println("============================");
    Serial.println("     CRABSENSE HARDWARE");
    Serial.println("============================");

    Serial.println("1 = Test bơm nguồn     (MOSFET IN2 / GPIO4)");
    Serial.println("2 = Test NO2 pump #1   (MOSFET IN3 / GPIO5)");
    Serial.println("3 = Test NO2 pump #2   (MOSFET IN1 / GPIO6)");
    Serial.println("4 = Test drain valve");
    Serial.println("5 = Run full NO2 test");
    Serial.println("0 = EMERGENCY STOP");

    Serial.println("============================");
}


void emergencyStop()
{
    stopAllPumps();

    closeDrainValve();

    Serial.println();
    Serial.println("!!! ALL DEVICES OFF !!!");
}


void setup()
{
    Serial.begin(115200);

    delay(1500);

    setupPumps();

    setupRelay();

    emergencyStop();

    Serial.println();
    Serial.println("CrabSense initialized.");

    printMenu();
}


void loop()
{
    if (!Serial.available())
    {
        return;
    }

    char command = Serial.read();

    if (command == '\r' || command == '\n' || command == ' ')
    {
        return;
    }

    switch (command)
    {
        case '1':

            Serial.println("TEST: BOM NGUON  MOSFET IN2  GPIO4");

            runPumpFor(
                PIN_SAMPLE_PUMP,
                3000
            );

            break;


        case '2':

            Serial.println("TEST: NO2 #1  MOSFET IN3  GPIO5  4 drops");

            doseDrops(
                PIN_NO2_REAGENT_1_PUMP,
                NO2_REAGENT_1_DROPS,
                REAGENT_1_DROP_TIME_MS
            );

            break;


        case '3':

            Serial.println("TEST: NO2 #2  MOSFET IN1  GPIO6  4 drops");

            doseDrops(
                PIN_NO2_REAGENT_2_PUMP,
                NO2_REAGENT_2_DROPS,
                REAGENT_2_DROP_TIME_MS
            );

            break;


        case '4':

            Serial.println("TEST: DRAIN VALVE");

            drainFor(2000);

            break;


        case '5':

            runNO2Test();

            break;


        case '0':

            emergencyStop();

            break;
    }

    printMenu();
}