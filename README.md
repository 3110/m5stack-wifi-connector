# M5Stack WiFi Connector

M5Stackデバイス向けのWiFi接続管理ライブラリです。複数の接続方法をサポートし、WiFiネットワークへの接続を簡単に管理できます。

## 特徴

- さまざまな接続情報の設定方法をサポート
  - NVS（Non-Volatile Storage）に保存された認証情報による接続
  - 手動で指定した認証情報による接続
  - シリアルモニタ経由での設定
  - SmartConfigによる設定
  - JSONファイルでの設定
- 上記の接続方法を組み合わせて設定可能
- コールバック機能によるイベント通知

## インストール

PlatformIO IDE環境でコンパイルします。`platformio.ini`ファイルに依存関係を追加します：

```ini
lib_deps = 
    m5stack/M5Unified
    https://github.com/3110/M5Stack-WiFi-Connector.git
```

## 基本的な使い方

`WiFiConnector`のインスタンスを用意し，`begin()`メソッドを呼び出すことでWiFi接続を開始します。`update()`メソッドを定期的に呼び出すことで，接続が切れた場合も再接続します。

WiFiの接続情報の設定方法は`setup`フォルダにある`WiFiSetupMethod`を継承したクラスのインスタンスを`addSetupMethod`メソッドに渡して追加します。明示的に設定方法を追加しなかった場合は，NVSに保存されたWiFi認証情報を使用します（`StoredCredentialSetup`が自動的に追加されます）。

以下は`M5Unified`を使用した基本的なサンプルコードです。

```cpp
#include <M5Unified.h>

#include "WiFiConnector.hpp"

WiFiConnector connector;

void setup(void) {
    M5.begin();

    // 以下を実行したのと同じ挙動になる
    // connector.addSetupMethod(new StoredCredentialSetup());
    
    if (!connector.begin()) {
        M5_LOGE("Failed to Connedt to WiFi");
    }
}

void loop(void) {
    M5.update();
    connector.update();
}
```

## 応用例

### 複数の接続方法を使用

`WiFiConnector`のインタンスを作成し，`aadSetupMethod`メソッドを使用して複数の接続方法を追加すると，接続できるまで接続方法を順番に試行します。

以下の例では，最初にNVSに保存されたWiFi認証情報を使用し，次にSmartConfigによる接続情報の設定を試みます。SmartConfigによる設定に失敗した場合は，シリアルモニタ経由での設定を行います。

```cpp
#include <M5Unified.h>

#include "WiFiConnector.hpp"
#include "setup/ManualCredentialSetup.hpp"
#include "setup/SerialConsoleSetup.hpp"
#include "setup/SmartConfigSetup.hpp"
#include "setup/StoredCredentialSetup.hpp"

#ifdef DEBUG
#define M5_LOG_LEVEL esp_log_level_t::ESP_LOG_DEBUG
#else
#define M5_LOG_LEVEL esp_log_level_t::ESP_LOG_INFO
#endif

WiFiConnector connector;

void setup(void) {
    auto cfg = M5.config();
    cfg.serial_baudrate = 115200;
    M5.begin(cfg);

    M5.Log.setLogLevel(m5::log_target_serial, M5_LOG_LEVEL);

    connector.addSetupMethod(new StoredCredentialSetup());
    connector.addSetupMethod(new SmartConfigSetup());
    connector.addSetupMethod(new SerialConsoleSetup(Serial));

    if (!connector.begin()) {
        M5_LOGE("Failed to connect to WiFi");
    }
}

void loop(void) {
    M5.update();
    connector.update();
}
```

### コールバックの使用

接続状態の変更を通知するためのコールバック機能を利用できます。以下の例では、WiFi接続の各状態に対応するコールバック関数を定義しています：
- 接続中：`onConnecting()` - 接続試行中に呼び出されます
- 接続完了：`onConnected()` - 接続成功時に呼び出されます
- 切断時：`onDisconnected()` - WiFiが切断された時に呼び出されます

これらのコールバック関数は、`WiFiConnector`インスタンスの以下のメソッドで設定できます：
- `setConnectingCallback()` - 接続試行中のコールバック設定
- `setConnectedCallback()` - 接続成功時のコールバック設定
- `setDisconnectedCallback()` - 切断時のコールバック設定

```cpp
#include <M5Unified.h>
#include "WiFiConnector.hpp"

WiFiConnector connector;

void onConnecting(unsigned long retries) {
    M5_LOGI("Connecting... (%lu time%s)\n", retries, (retries == 1) ? "" : "s");
}

void onConnected(String ssid, String password) {
    M5_LOGI("SSID: %s", ssid.c_str());
    M5_LOGI("IP Address: %s", connector.getIPAddress().c_str());
}

void onDisconnected(void) {
	M5_LOGI("Disconnected from WiFi");
}

void setup() {
    M5.begin();
    
    connector.setConnectingCallback(onConnecting);
    connector.setConnectedCallback(onConnected);
    connector.setDisconnectedCallback(onDisconnected);
    
    if (!connector.begin()) {
        M5_LOGE("Failed to connect to WiFi");
    }
}

void loop() {
    M5.update();
    connector.update();
}
```

## WiFi情報の設定方法

`WiFiConnector`クラスは複数の接続方法をサポートしています。以下の接続方法があります。

- 保存された認証情報を使用（`StoredCredentialSetup`）
- シリアル経由での設定（`SerialConsoleSetup`）
- 手動での指定（`ManualCredentialSetup`）
- JSONファイルからの設定（`JsonCredentialSetup`）
- SmartConfigによる設定（`SmartConfigSetup`）

先に説明したように，複数の設定方法を組み合わせて使用することも可能です。接続方法は順番に試行され，最初に成功した方法が使用されます。

### `StoredCredentialSetup`: NVSに保存された認証情報を使用

NVS（Non-Volatile Storage）に保存されたWiFi認証情報を使用して接続します。

### `SerialConsoleSetup(Serial)`: シリアルモニタ経由での設定

シリアルモニタ経由でSSIDとパスワードを手動で入力します。パスワードはエコーバックするようにしているので注意してください。

```shell
== Serial Console Setup ==
SSID: 
Password: 
```

### `ManualCredentialSetup`: 手動で認証情報を設定

引数として渡したSSIDとパスワードで接続します。

```cpp
connector.addSetupMethod(new ManualCredentialSetup("SSID", "パスワード"));
```

### `JSONCredentialSetup`: 指定したファイルシステムにあるJSONファイルで設定

指定したファイルシステムに置かれたJSONファイルからWiFi設定を読み込みます。

以下の例ではLittleFS上にある`wifi.json`からWiFi設定を読み込みます。

```cpp
connector.addSetupMethod(new JSONCredentialSetup(LittleFS, "/wifi.json"));
```

`wifi.json`:
```json
{
  "ssid": "SSID",
  "password": "パスワード"
}
```

### `SmartConfigSetup`: SmartConfigによる設定

スマートフォンのESP Touchアプリを使用して設定します。

## サンプルコード

`examples`フォルダに以下のサンプルが含まれています：

- `advanced_setup` - 複数の接続方法の組み合わせ
- `basic_setup` - 基本的な使い方
- `json_setup` - JSONファイルからの設定読み込み
- `serial_setup` - シリアル経由での設定
- `smart_config_setup` - SmartConfigの使用
