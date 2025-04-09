#pragma once

#include "setup/WiFiSetupMethod.hpp"

class ManualCredentialSetup : public WiFiSetupMethod {
public:
    ManualCredentialSetup(const char* ssid, const char* password);
    virtual ~ManualCredentialSetup(void) = default;

    virtual bool setup(void) override;
    virtual bool connect(void) override;

private:
    String _ssid;
    String _password;
};