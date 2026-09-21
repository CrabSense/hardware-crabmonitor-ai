#ifndef SETTINGS_H
#define SETTINGS_H

// 5V MOSFET/relay optocoupler modules are active-LOW.
// ESP32 GPIO HIGH is only 3.3V, so "digital HIGH" does NOT turn the
// opto off — the pump stays ON. OFF must release the pin (high-Z)
// so the module's 5V pull-up can fully extinguish the LED.
#define MOSFET_ON  LOW
#define RELAY_ON   LOW

// SAMPLE
#define SAMPLE_PUMP_TIME_MS 4000

// NO2 reagent
#define NO2_REAGENT_1_DROPS 4
#define NO2_REAGENT_2_DROPS 4

#define REAGENT_1_DROP_TIME_MS 200
#define REAGENT_2_DROP_TIME_MS 200

#define DROP_INTERVAL_MS 1000

// Reaction
#define WAIT_AFTER_REAGENT_1_MS 3000
#define NO2_REACTION_TIME_MS 10000

// Drain
#define DRAIN_TIME_MS 5000

#endif