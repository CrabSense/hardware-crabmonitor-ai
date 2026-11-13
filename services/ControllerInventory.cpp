#include "ControllerInventory.h"
#include "../config/Pins.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

namespace {
struct Port {
    const char* id;
    int gpio;
    const char* kind;
};

// Known wiring from Pins.h, NOT electrical detection of external loads.
// NO2 actuators remain local-only to protect the measurement sequence.
const Port ports[] = {
    {"sample-pump", PIN_SAMPLE_PUMP, "mosfet_pump"},
    {"no2-reagent-1", PIN_NO2_REAGENT_1_PUMP, "mosfet_pump"},
    {"no2-reagent-2", PIN_NO2_REAGENT_2_PUMP, "mosfet_pump"},
    {"drain-valve", PIN_DRAIN_RELAY, "relay_valve"}
};

String portJson() {
    String result = "[";
    bool first = true;
    for (const auto& p : ports) {
        if (!first) result += ",";
        first = false;
        result += "{\"portId\":\"";
        result += p.id;
        result += "\",\"gpio\":";
        result += String(p.gpio);
        result += ",\"kind\":\"";
        result += p.kind;
        result += "\",\"direction\":\"output\",\"discovery\":\"declared\","
                  "\"present\":true,\"remoteControl\":false}";
    }
    result += "]";
    return result;
}

bool safeCode(const String& code) {
    if (code.isEmpty() || code.length() > 100) return false;
    for (unsigned i = 0; i < code.length(); ++i) {
        char c = code[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9') || c == '_' || c == '-' || c == '.' || c == ':'))
            return false;
    }
    return code != "." && code != "..";
}

String revisionText(uint64_t revision) {
    char buffer[24];
    snprintf(buffer, sizeof(buffer), "%llu", (unsigned long long)revision);
    return String(buffer);
}

// One NVS value keeps the revision and exact descriptor snapshot together.
// Same wiring => same revision across reboots; changed descriptor => next revision.
String makeManifest(Preferences& prefs) {
    const String payload = portJson();
    const String previous = prefs.getString("snapshot", "");
    const int separator = previous.indexOf('|');
    uint64_t revision = 1;
    if (separator > 0) {
        revision = strtoull(previous.substring(0, separator).c_str(), nullptr, 10);
        if (revision == 0 || revision >= INT64_MAX) return "";
        if (previous.substring(separator + 1) != payload) ++revision;
    }
    const String state = revisionText(revision) + "|" + payload;
    if (state != previous && prefs.putString("snapshot", state) == 0) return "";
    return String("{\"schemaVersion\":1,\"revision\":") + revisionText(revision)
        + ",\"ports\":" + payload + "}";
}

int sendManifest(WiFiClient& client, const String& url, const String& key, const String& body) {
    HTTPClient http;
    if (!http.begin(client, url)) return -1;
    http.setConnectTimeout(2500);
    http.setTimeout(4000);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Device-Key", key);
    const int status = http.PUT(body);
    http.end();
    return status;
}

void inventoryTask(void*) {
    Preferences prefs;
    if (!prefs.begin("crab-inventory", false)) {
        vTaskDelete(nullptr);
        return;
    }
    String kiosk = prefs.getString("kiosk", "");
    const String code = prefs.getString("device_code", "");
    const String key = prefs.getString("device_key", "");
    const String ca = prefs.getString("ca", "");
    const bool devHttp = prefs.getBool("dev_http", false);
    const bool tls = kiosk.startsWith("https://");
    if (!safeCode(code) || key.isEmpty() || kiosk.isEmpty()
        || (tls && ca.isEmpty()) || (!tls && !(devHttp && kiosk.startsWith("http://")))) {
        prefs.end();
        vTaskDelete(nullptr);
        return;
    }
    const String manifest = makeManifest(prefs);
    prefs.end();
    if (manifest.isEmpty()) {
        vTaskDelete(nullptr);
        return;
    }
    while (kiosk.endsWith("/")) kiosk.remove(kiosk.length() - 1);
    const String url = kiosk + "/api/controllers/" + code + "/ports/manifest";
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    if (WiFi.status() != WL_CONNECTED) WiFi.begin(); // use previously provisioned Wi-Fi

    for (;;) {
        int status = -1;
        if (WiFi.status() == WL_CONNECTED) {
            if (tls) {
                WiFiClientSecure client;
                client.setCACert(ca.c_str());
                status = sendManifest(client, url, key, manifest);
            } else {
                // Explicit development opt-in only. Credentials are plaintext on HTTP.
                WiFiClient client;
                status = sendManifest(client, url, key, manifest);
            }
            Serial.printf("Controller inventory HTTP status: %d\n", status);
        }
        const bool success = status >= 200 && status < 300;
        vTaskDelay(pdMS_TO_TICKS(success ? 60000 : 10000));
    }
}
} // namespace

void startControllerInventoryReporter() {
    // Network timeouts must not block the Serial emergency stop or NO2 sequence.
    static bool started = false;
    if (!started) {
        started = xTaskCreate(inventoryTask, "crab-inventory", 8192, nullptr, 1, nullptr) == pdPASS;
    }
}
