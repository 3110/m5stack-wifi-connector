#pragma once

#include <Arduino.h>
#include <WiFi.h>

class WiFiSetupMethod {
public:
    static const size_t MAX_SSID_LENGTH = 32;
    static const size_t MAX_PASSWORD_LENGTH = 64;

    WiFiSetupMethod(const char* name) : _name(name) {
        if (name == nullptr || strlen(name) == 0) {
            log_e("name is null or empty");
        }
    }
    virtual ~WiFiSetupMethod(void) = default;

    virtual bool setup(void) = 0;
    virtual bool connect(void) = 0;

    virtual const String& getName(void) const {
        return this->_name;
    }

private:
    String _name;
};