#ifndef PINS_H
#define PINS_H

// MOSFET module (confirmed on hardware):
//   IN2 <- GPIO4 , OUT2 -> bơm nguồn (sample)
//   IN3 <- GPIO5 , OUT3 -> bơm NO2 #1
//   IN1 <- GPIO6 , OUT1 -> bơm NO2 #2

#define PIN_SAMPLE_PUMP 4
#define PIN_NO2_PUMP_1 5
#define PIN_NO2_REAGENT_1_PUMP PIN_NO2_PUMP_1
#define PIN_NO2_PUMP_2 6
#define PIN_NO2_REAGENT_2_PUMP PIN_NO2_PUMP_2

// Van xả
// ESP32 GPIO7 -> Relay IN1
#define PIN_DRAIN_RELAY 7

#endif