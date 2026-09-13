#include <Arduino.h>

#include "../../config/Pins.h"
#include "../../config/Settings.h"

#include "../../drivers/DeviceController.h"

#include "NO2Test.h"


// ================================
// STEP 1
// LẤY NƯỚC
// ================================

void takeWater() {

    Serial.println("STEP 1: TAKE WATER");

    deviceOn(SAMPLE_PUMP);

    delay(SAMPLE_WATER_TIME);

    deviceOff(SAMPLE_PUMP);
}


// ================================
// STEP 2
// DUNG DỊCH NO2 1
// ================================

void addReagent1() {

    Serial.println("STEP 2: ADD REAGENT 1");

    deviceOn(NO2_PUMP_1);

    delay(NO2_REAGENT_1_TIME);

    deviceOff(NO2_PUMP_1);
}


// ================================
// STEP 3
// DUNG DỊCH NO2 2
// ================================

void addReagent2() {

    Serial.println("STEP 3: ADD REAGENT 2");

    deviceOn(NO2_PUMP_2);

    delay(NO2_REAGENT_2_TIME);

    deviceOff(NO2_PUMP_2);
}


// ================================
// STEP 4
// WAIT REACTION
// ================================

void waitForReaction() {

    Serial.println("STEP 4: WAIT FOR COLOR");

    delay(NO2_REACTION_TIME);
}


// ================================
// STEP 5
// AI ANALYSIS
// ================================

void analyzeNO2() {

    Serial.println("STEP 5: REQUEST AI ANALYSIS");

    // Sau này gọi AIService ở đây
}


// ================================
// STEP 6
// XẢ
// ================================

void drainTestWater() {

    Serial.println("STEP 6: DRAIN WATER");

    deviceOn(DRAIN_VALVE);

    delay(DRAIN_TIME);

    deviceOff(DRAIN_VALVE);
}


// ================================
// FULL PROCESS
// ================================

void runNO2Test() {

    Serial.println("");
    Serial.println("======================");
    Serial.println("NO2 TEST START");
    Serial.println("======================");

    takeWater();

    addReagent1();

    delay(3000);

    addReagent2();

    waitForReaction();

    analyzeNO2();

    drainTestWater();

    Serial.println("NO2 TEST FINISHED");
}