// clang-format off
#include <LittleFS.h>
#define TARGET_FS LittleFS
#include <M5Unified.h>
// clang-format on

#include "WiFiConnector.hpp"
#include "setup/JSONCredentialSetup.hpp"

#ifdef DEBUG
#define M5_LOG_LEVEL esp_log_level_t::ESP_LOG_DEBUG
#else
#define M5_LOG_LEVEL esp_log_level_t::ESP_LOG_INFO
#endif

static constexpr const char* CREDENTIAL_FILENAME = "/wifi.json";
static constexpr bool FORMAT_FS_IF_FAILED = true;

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

    if (!TARGET_FS.begin(FORMAT_FS_IF_FAILED)) {
        M5_LOGE("Failed to mount LittleFS");
        return;
    }

    connector.setConnectingCallback(onConnecting);
    connector.setConnectedCallback(onConnected);
    connector.setDisconnectedCallback(onDisconnected);

    connector.addSetupMethod(
        new JSONCredentialSetup(TARGET_FS, CREDENTIAL_FILENAME));
    if (!connector.begin()) {
        M5_LOGE("Failed to connect to WiFi");
    }
}

void loop(void) {
    M5.update();
    connector.update();
}