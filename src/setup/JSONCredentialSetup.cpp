#include "setup/JSONCredentialSetup.hpp"

static constexpr const char* NAME = "JSON Credential Setup";

const char* JSONCredentialSetup::KEY_SSID = "ssid";
const char* JSONCredentialSetup::KEY_PASSWORD = "password";

JSONCredentialSetup::JSONCredentialSetup(FS& fs, const char* filename)
    : WiFiSetupMethod(NAME),
      _fs(fs),
      _filename(filename),
      _ssid(""),
      _password("") {
}

bool JSONCredentialSetup::setup(void) {
    log_i("Reading credentials from %s", this->_filename.c_str());
    File file = _fs.open(this->_filename, FILE_READ);
    if (!file) {
        log_e("Failed to open file for reading");
        return false;
    }

    String jsonString = "";
    while (file.available()) {
        jsonString += static_cast<char>(file.read());
    }
    file.close();

    JSONVar config = JSON.parse(jsonString);
    if (JSON.typeof(config) == "undefined") {
        log_e("Failed to parse JSON");
        return false;
    }
    if (!config.hasOwnProperty(KEY_SSID)) {
        log_e("Missing SSID in JSON");
        return false;
    }
    if (!config.hasOwnProperty(KEY_PASSWORD)) {
        log_e("Missing password in JSON");
        return false;
    }

    const char* ssid = static_cast<const char*>(config[KEY_SSID]);
    if (ssid == nullptr || strlen(ssid) == 0 ||
        strlen(ssid) > MAX_SSID_LENGTH) {
        log_e("SSID too long or missing!");
        return false;
    }

    const char* password = static_cast<const char*>(config[KEY_PASSWORD]);
    if (password == nullptr || strlen(password) > MAX_PASSWORD_LENGTH) {
        log_e("Password too long!");
        return false;
    }

    this->_ssid = ssid;
    this->_password = password;

    return true;
}

bool JSONCredentialSetup::connect(void) {
    WiFi.begin(this->_ssid.c_str(), this->_password.c_str());
    return true;
}