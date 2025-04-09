#include "setup/StoredCredentialSetup.hpp"

static constexpr const char* NAME = "Stored Credential Setup";

StoredCredentialSetup::StoredCredentialSetup(void) : WiFiSetupMethod(NAME) {
}

bool StoredCredentialSetup::setup(void) {
    return true;
}

bool StoredCredentialSetup::connect(void) {
    WiFi.begin();
    return true;
}