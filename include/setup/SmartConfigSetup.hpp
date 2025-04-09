#pragma once

#include "setup/WiFiSetupMethod.hpp"

class SmartConfigSetup : public WiFiSetupMethod {
public:
    static constexpr unsigned long SMART_CONFIG_TIMEOUT_MS = 30000;
    static constexpr unsigned long SMART_CONFIG_RETRY_INTERVAL_MS = 100;

    SmartConfigSetup(void);
    virtual ~SmartConfigSetup(void) = default;

    virtual bool setup(void) override;
    virtual bool connect(void) override;
};