#include "PumpController.h"

#include "../config/Pins.h"
#include "../config/Settings.h"

#include "driver/gpio.h"

static const char* pumpName(uint8_t pin)
{
    if (pin == PIN_SAMPLE_PUMP)
    {
        return "SAMPLE IN2/OUT2 GPIO4";
    }

    if (pin == PIN_NO2_PUMP_1)
    {
        return "NO2#1 IN3/OUT3 GPIO5";
    }

    if (pin == PIN_NO2_PUMP_2)
    {
        return "NO2#2 IN1/OUT1 GPIO6";
    }

    return "UNKNOWN";
}

static void drivePumpOn(uint8_t pin)
{
    const gpio_num_t gpio = static_cast<gpio_num_t>(pin);

    gpio_hold_dis(gpio);
    gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio, 0);
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

static void drivePumpOff(uint8_t pin)
{
    const gpio_num_t gpio = static_cast<gpio_num_t>(pin);

    // High-Z so a 5V opto input can rise to 5V and actually turn OFF.
    digitalWrite(pin, HIGH);
    pinMode(pin, INPUT);
    gpio_set_direction(gpio, GPIO_MODE_INPUT);
    gpio_pulldown_dis(gpio);
    gpio_pullup_dis(gpio);
}

static void setupPumpPin(uint8_t pin)
{
    const gpio_num_t gpio = static_cast<gpio_num_t>(pin);

    gpio_reset_pin(gpio);
    gpio_hold_dis(gpio);
    drivePumpOff(pin);
}

void setupPumps()
{
    setupPumpPin(PIN_SAMPLE_PUMP);
    setupPumpPin(PIN_NO2_PUMP_1);
    setupPumpPin(PIN_NO2_PUMP_2);

    stopAllPumps();
}

void pumpOn(uint8_t pin)
{
    drivePumpOn(pin);

    Serial.print(pumpName(pin));
    Serial.println(" ON  (GPIO sink LOW)");
}

void pumpOff(uint8_t pin)
{
    drivePumpOff(pin);

    Serial.print(pumpName(pin));
    Serial.println(" OFF (GPIO high-Z)");
}

void stopAllPumps()
{
    pumpOff(PIN_SAMPLE_PUMP);
    pumpOff(PIN_NO2_PUMP_1);
    pumpOff(PIN_NO2_PUMP_2);
}

void runPumpFor(uint8_t pin, unsigned long durationMs)
{
    pumpOn(pin);

    delay(durationMs);

    pumpOff(pin);
}

void doseDrops(
    uint8_t pin,
    int drops,
    unsigned long dropTimeMs
)
{
    for (int i = 0; i < drops; i++)
    {
        Serial.print("Drop ");
        Serial.print(i + 1);
        Serial.print("/");
        Serial.println(drops);

        runPumpFor(pin, dropTimeMs);

        delay(DROP_INTERVAL_MS);
    }
}