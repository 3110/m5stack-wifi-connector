#include "setup/SerialConsoleSetup.hpp"

static constexpr const char* NAME = "Serial Console Setup";

static constexpr const char* SERIAL_SETUP_TITLE = "== Serial Console Setup ==";
static constexpr const char* SSID_PROMPT = "SSID: ";
static constexpr const char* PASSWORD_PROMPT = "Password: ";
static constexpr const char* ERROR_SSID_EMPTY = "Error: SSID cannot be empty";
static constexpr const char* EMPTY_PASSWORD = "<empty>";

SerialConsoleSetup::SerialConsoleSetup(Stream& serial)
    : WiFiSetupMethod(NAME), _serial(serial), _ssid(), _password() {
}

bool SerialConsoleSetup::setup(void) {
    this->_serial.println(SERIAL_SETUP_TITLE);
    this->_serial.print(SSID_PROMPT);
    this->_ssid = readLine();
    if (this->_ssid.isEmpty()) {
        this->_serial.println(ERROR_SSID_EMPTY);
        return false;
    }
    this->_serial.println(this->_ssid);

    this->_serial.print(PASSWORD_PROMPT);
    this->_password = readLine();
    this->_serial.println(this->_password.isEmpty() ? EMPTY_PASSWORD
                                                    : this->_password);
    return true;
}

bool SerialConsoleSetup::connect(void) {
    if (this->_ssid.isEmpty()) {
        log_e("SSID is empty");
        return false;
    }
    WiFi.begin(this->_ssid.c_str(), this->_password.c_str());
    return true;
}

String SerialConsoleSetup::readLine(void) {
    const unsigned long startTime = millis();
    while (!this->_serial.available() &&
           millis() - startTime < INPUT_TIMEOUT_MS) {
        delay(INPUT_RETRY_INTERVAL_MS);
    }
    String line = "";
    if (this->_serial.available()) {
        line = this->_serial.readStringUntil('\n');
        line.trim();
    }
    return line;
}
