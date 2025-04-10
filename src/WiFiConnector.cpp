#include "WiFiConnector.hpp"

#include "setup/StoredCredentialSetup.hpp"

static const char* NOT_CONNECTED = "Not connected";

WiFiConnector::WiFiConnector(void)
    : _connected(false),
      _setupMethodCount(0),
      _connectionTimeout(0),
      _connectionDelay(0),
      _connectionRetryInterval(0),
      _connectingCallback(nullptr),
      _connectedCallback(nullptr),
      _disconnectedCallback(nullptr) {
    for (size_t i = 0; i < MAX_SETUP_METHODS; i++) {
        this->_setupMethods[i] = nullptr;
    }
}

WiFiConnector::~WiFiConnector(void) {
    WiFi.disconnect(true);
    for (size_t p = 0; p < this->_setupMethodCount; ++p) {
        delete this->_setupMethods[p];
        this->_setupMethods[p] = nullptr;
    }
}

bool WiFiConnector::addSetupMethod(WiFiSetupMethod* method) {
    if (method == nullptr) {
        log_e("Setup method is null");
        return false;
    }
    if (method->getName().isEmpty()) {
        log_e("Setup name is empty");
        delete method;
        return false;
    }
    if (this->_setupMethodCount < MAX_SETUP_METHODS) {
        this->_setupMethods[this->_setupMethodCount++] = method;
        log_i("Added setup method: %s", method->getName().c_str());
        return true;
    } else {
        log_e("Max setup methods reached");
        delete method;
        return false;
    }
}

bool WiFiConnector::begin(unsigned long connectionTimeout,
                          unsigned long connectionDelay,
                          unsigned long connectionRetryInterval) {
    if (this->_setupMethodCount == 0) {
        log_w("No setup methods fouud.");
        log_w("Adding default setup method: StoredCredentialSetup");
        addSetupMethod(new StoredCredentialSetup());
    }
    this->_connectionTimeout = connectionTimeout;
    this->_connectionDelay = connectionDelay;
    this->_connectionRetryInterval = connectionRetryInterval;

    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    delay(this->_connectionDelay);

    WiFiSetupMethod* method = nullptr;
    const char* name = nullptr;
    for (size_t p = 0; p < this->_setupMethodCount; ++p) {
        method = this->_setupMethods[p];
        name = method->getName().c_str();
        log_i("Trying setup method: %s", name);
        if (!method->setup()) {
            log_e("Failed to setup with %s", name);
            continue;
        }
        if (!method->connect()) {
            log_e("Failed to connect with %s", name);
            WiFi.disconnect(true);
            continue;
        }
        if (waitConnection()) {
            return true;
        }
    }
    return false;
}

void WiFiConnector::setConnectingCallback(ConnectingCallback callback) {
    this->_connectingCallback = callback;
}

void WiFiConnector::setConnectedCallback(ConnectedCallback callback) {
    this->_connectedCallback = callback;
}

void WiFiConnector::setDisconnectedCallback(DisconnectedCallback callback) {
    this->_disconnectedCallback = callback;
}

bool WiFiConnector::waitConnection(void) {
    const unsigned long startTime = millis();
    unsigned long retries = 0;
    while (WiFi.status() != WL_CONNECTED &&
           millis() - startTime < this->_connectionTimeout) {
        delay(this->_connectionRetryInterval);
        if (WiFi.status() == WL_NO_SSID_AVAIL) {
            log_e("SSID is not available");
            break;
        }
        ++retries;
        if (this->_connectingCallback != nullptr) {
            this->_connectingCallback(retries);
        }
    }
    this->_connected = WiFi.status() == WL_CONNECTED;
    if (this->_connected && this->_connectedCallback != nullptr) {
        this->_connectedCallback();
    }
    return this->_connected;
}

bool WiFiConnector::update(void) {
    if (WiFi.status() != WL_CONNECTED) {
        if (this->_connected) {
            this->_connected = false;
            if (this->_disconnectedCallback != nullptr) {
                this->_disconnectedCallback();
            }
        }
        WiFi.reconnect();
    } else {
        if (!this->_connected) {
            if (this->_connectedCallback != nullptr) {
                this->_connectedCallback();
            }
            this->_connected = true;
            return true;
        }
    }
    return false;
}

bool WiFiConnector::isConnected(void) const {
    return this->_connected;
}

String WiFiConnector::getIPAddress(void) const {
    return isConnected() ? WiFi.localIP().toString() : NOT_CONNECTED;
}

String WiFiConnector::getSSID(void) const {
    return isConnected() ? WiFi.SSID() : NOT_CONNECTED;
}
