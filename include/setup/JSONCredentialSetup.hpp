#pragma once

#include <Arduino_JSON.h>
#include <FS.h>

#include "WiFiSetupMethod.hpp"

class JSONCredentialSetup : public WiFiSetupMethod {
public:
    static const char* KEY_SSID;
    static const char* KEY_PASSWORD;

    JSONCredentialSetup(FS& fs, const char* filename);
    virtual ~JSONCredentialSetup(void) = default;

    virtual bool setup(void) override;
    virtual bool connect(void) override;

private:
    FS& _fs;
    String _filename;
    String _ssid;
    String _password;
};