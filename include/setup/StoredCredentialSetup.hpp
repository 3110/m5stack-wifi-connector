#pragma once

#include "setup/WiFiSetupMethod.hpp"

class StoredCredentialSetup : public WiFiSetupMethod {
public:
    static constexpr unsigned long CONNECTION_TIMEOUT_MS = 10000;
    static constexpr unsigned long CONNECTION_RETRY_INTERVAL_MS = 500;

    StoredCredentialSetup(void);
    virtual ~StoredCredentialSetup(void) = default;

    virtual bool setup(void) override;
    virtual bool connect(void) override;
};