#include "RelayController.h"

#include "../config/Pins.h"
#include "../config/Settings.h"

#include "driver/gpio.h"

static void driveRelayOn()
{
    const gpio_num_t gpio = static_cast<gpio_num_t>(PIN_DRAIN_RELAY);

    gpio_hold_dis(gpio);
    gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio, 0);
    pinMode(PIN_DRAIN_RELAY, OUTPUT);
    digitalWrite(PIN_DRAIN_RELAY, LOW);
}

static void driveRelayOff()
{
    const gpio_num_t gpio = static_cast<gpio_num_t>(PIN_DRAIN_RELAY);

    digitalWrite(PIN_DRAIN_RELAY, HIGH);
    pinMode(PIN_DRAIN_RELAY, INPUT);
    gpio_set_direction(gpio, GPIO_MODE_INPUT);
    gpio_pulldown_dis(gpio);
    gpio_pullup_dis(gpio);
}

void setupRelay()
{
    const gpio_num_t gpio = static_cast<gpio_num_t>(PIN_DRAIN_RELAY);

    gpio_reset_pin(gpio);
    gpio_hold_dis(gpio);
    closeDrainValve();
}

void openDrainValve()
{
    driveRelayOn();

    Serial.println("Drain valve: OPEN (GPIO7 sink LOW)");
}

void closeDrainValve()
{
    driveRelayOff();

    Serial.println("Drain valve: CLOSED (GPIO7 high-Z)");
}

void drainFor(unsigned long durationMs)
{
    openDrainValve();

    delay(durationMs);

    closeDrainValve();
}