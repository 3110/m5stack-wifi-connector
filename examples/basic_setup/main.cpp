#include <M5Unified.h>

#include "WiFiConnector.hpp"

#ifdef DEBUG
#define M5_LOG_LEVEL esp_log_level_t::ESP_LOG_DEBUG
#else
#define M5_LOG_LEVEL esp_log_level_t::ESP_LOG_INFO
#endif

WiFiConnector connector;

void onConnecting(unsigned long retries) {
    M5_LOGI("Connecting to WiFi... (%lu time%s)", retries,
            (retries == 1) ? "" : "s");
}

void onConnected(void) {
    M5_LOGI("Connected.");
    M5_LOGI("  SSID: %s", connector.getSSID().c_str());
    M5_LOGI("  IP Address: %s", connector.getIPAddress().c_str());
}

void onDisconnected(void) {
    M5_LOGI("Disconnected from WiFi");
}

void setup(void) {
    auto cfg = M5.config();
    cfg.serial_baudrate = 115200;
    M5.begin(cfg);

    M5.Log.setLogLevel(m5::log_target_serial, M5_LOG_LEVEL);

    connector.setConnectingCallback(onConnecting);
    connector.setConnectedCallback(onConnected);
    connector.setDisconnectedCallback(onDisconnected);

    if (!connector.begin()) {
        M5_LOGE("Failed to Connedt to WiFi");
    }
}

void loop(void) {
    M5.update();
    connector.update();
}