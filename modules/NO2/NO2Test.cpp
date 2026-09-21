#include <Arduino.h>

#include "NO2Test.h"

#include "../../config/Pins.h"
#include "../../config/Settings.h"

#include "../../drivers/PumpController.h"
#include "../../drivers/RelayController.h"

// ======================================
// STEP 1 - TAKE WATER SAMPLE
// ======================================

void takeWaterSample()
{
    Serial.println();
    Serial.println("[NO2] STEP 1");
    Serial.println("Taking 5 ml water sample...");

    runPumpFor(
        PIN_SAMPLE_PUMP,
        SAMPLE_PUMP_TIME_MS
    );

    Serial.println("Water sample completed.");
}


// ======================================
// STEP 2 - REAGENT 1
// ======================================

void addNO2Reagent1()
{
    Serial.println();
    Serial.println("[NO2] STEP 2");
    Serial.println("Adding reagent 1...");

    doseDrops(
        PIN_NO2_REAGENT_1_PUMP,
        NO2_REAGENT_1_DROPS,
        REAGENT_1_DROP_TIME_MS
    );

    Serial.println("Reagent 1 completed.");
}


// ======================================
// STEP 3 - REAGENT 2
// ======================================

void addNO2Reagent2()
{
    Serial.println();
    Serial.println("[NO2] STEP 3");
    Serial.println("Adding reagent 2...");

    doseDrops(
        PIN_NO2_REAGENT_2_PUMP,
        NO2_REAGENT_2_DROPS,
        REAGENT_2_DROP_TIME_MS
    );

    Serial.println("Reagent 2 completed.");
}


// ======================================
// STEP 4 - WAIT FOR COLOR
// ======================================

void waitForNO2Reaction()
{
    Serial.println();
    Serial.println("[NO2] STEP 4");
    Serial.println("Waiting for reaction...");

    delay(NO2_REACTION_TIME_MS);

    Serial.println("Reaction time completed.");
}


// ======================================
// STEP 5 - AI PLACEHOLDER
// ======================================

void requestNO2Analysis()
{
    Serial.println();
    Serial.println("[NO2] STEP 5");

    Serial.println(
        "AI analysis requested..."
    );

    // Chưa kết nối AI ở V1.
    // V2 sẽ gọi AIService ở đây.

    delay(1000);

    Serial.println(
        "AI placeholder completed."
    );
}


// ======================================
// STEP 6 - DRAIN
// ======================================

void drainNO2Sample()
{
    Serial.println();
    Serial.println("[NO2] STEP 6");
    Serial.println("Draining test sample...");

    drainFor(DRAIN_TIME_MS);

    Serial.println("Drain completed.");
}


// ======================================
// COMPLETE NO2 TEST
// ======================================

void runNO2Test()
{
    Serial.println();
    Serial.println("===========================");
    Serial.println("    CRABSENSE NO2 TEST");
    Serial.println("===========================");

    // Đảm bảo an toàn trước khi bắt đầu.
    stopAllPumps();
    closeDrainValve();

    // 1. Lấy mẫu
    takeWaterSample();

    // 2. Thuốc thử 1
    addNO2Reagent1();

    // Chờ theo quy trình hóa chất
    delay(WAIT_AFTER_REAGENT_1_MS);

    // 3. Thuốc thử 2
    addNO2Reagent2();

    // 4. Chờ phản ứng
    waitForNO2Reaction();

    // 5. Camera / AI
    requestNO2Analysis();

    // 6. Xả
    drainNO2Sample();

    // Đảm bảo tất cả OFF
    stopAllPumps();
    closeDrainValve();

    Serial.println();
    Serial.println("===========================");
    Serial.println("      NO2 TEST DONE");
    Serial.println("===========================");
}