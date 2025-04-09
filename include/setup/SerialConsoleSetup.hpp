#pragma once

#include <Print.h>
#include <Stream.h>

#include "setup/WiFiSetupMethod.hpp"

class SerialConsoleSetup : public WiFiSetupMethod {
public:
    static constexpr unsigned long INPUT_TIMEOUT_MS = 30000;
    static constexpr unsigned long INPUT_RETRY_INTERVAL_MS = 100;

    SerialConsoleSetup(Stream& serial);
    virtual ~SerialConsoleSetup(void) = default;

    virtual bool setup(void) override;
    virtual bool connect(void) override;

protected:
    virtual String readLine(void);

private:
    Stream& _serial;
    String _ssid;
    String _password;
};
