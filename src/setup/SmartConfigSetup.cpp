#include "setup/SmartConfigSetup.hpp"

static constexpr const char* NAME = "Smart Config Setup";

SmartConfigSetup::SmartConfigSetup(void) : WiFiSetupMethod(NAME) {
}

bool SmartConfigSetup::setup(void) {
    if (!WiFi.beginSmartConfig()) {
        log_e("Failed to start SmartConfig");
        return false;
    }
    log_i("Waiting for SmartConfig...");
    const unsigned long startTime = millis();
    while (!WiFi.smartConfigDone() &&
           millis() - startTime < SMART_CONFIG_TIMEOUT_MS) {
        delay(SMART_CONFIG_RETRY_INTERVAL_MS);
    }
    log_i("SmartConfig %s", WiFi.smartConfigDone() ? "done" : "failed");
    return WiFi.smartConfigDone();
}

bool SmartConfigSetup::connect(void) {
    return true;
}