#pragma once

#include "setup/WiFiSetupMethod.hpp"

class WiFiConnector {
public:
    typedef void (*ConnectingCallback)(unsigned long retries);
    typedef void (*ConnectedCallback)(void);
    typedef void (*DisconnectedCallback)(void);
    typedef void (*FailedCallback)(void);

    static const size_t MAX_SETUP_METHODS = 5;

    static const unsigned long CONNECTION_DELAY_MS = 1000UL;
    static const unsigned long CONNECTION_TIMEOUT_MS = 30000UL;
    static const unsigned long CONNECTION_RETRY_INTERVAL_MS = 500UL;

    WiFiConnector(void);
    virtual ~WiFiConnector(void);

    virtual bool addSetupMethod(WiFiSetupMethod* method);

    virtual bool begin(void);
    virtual void setConnectingCallback(ConnectingCallback callback);
    virtual void setConnectedCallback(ConnectedCallback callback);
    virtual void setDisconnectedCallback(DisconnectedCallback callback);
    virtual bool update(void);

    virtual bool isConnected(void) const;

    virtual String getIPAddress(void) const;
    virtual String getSSID(void) const;

protected:
    virtual bool waitConnection(void);

private:
    bool _connected;
    WiFiSetupMethod* _setupMethods[MAX_SETUP_METHODS];
    size_t _setupMethodCount;
    ConnectingCallback _connectingCallback;
    ConnectedCallback _connectedCallback;
    DisconnectedCallback _disconnectedCallback;
};
