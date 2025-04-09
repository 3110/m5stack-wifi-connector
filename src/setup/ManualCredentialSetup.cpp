#include "setup/ManualCredentialSetup.hpp"

static constexpr const char* NAME = "Manual Credential Setup";

ManualCredentialSetup::ManualCredentialSetup(const char* ssid,
                                             const char* password)
    : WiFiSetupMethod(NAME), _ssid(ssid), _password(password) {
}

bool ManualCredentialSetup::setup(void) {
    if (this->_ssid.isEmpty() || this->_ssid.length() > MAX_SSID_LENGTH) {
        return false;
    }
    if (this->_password.length() > MAX_PASSWORD_LENGTH) {
        return false;
    }
    return true;
}

bool ManualCredentialSetup::connect(void) {
    WiFi.begin(this->_ssid.c_str(), this->_password.c_str());
    return true;
}