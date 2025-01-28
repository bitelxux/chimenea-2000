//Libraries
#include <FS.h>
#include <ArduinoJson.h>  // 5.13.5 !!

#include "config.h"

// tried many approaches to #include "btlx25/btlx25.h"
// non worked. Workaround is to symlink the btlx25 folder to Arduino/libraries
#include "btlx25.h"

// html page for mp3 management
#include "web.h"

// Reset Button
#define RESET_BUTTON 14
unsigned long rb_last_change = 0;
int rb_required_time = 3;
boolean time_to_reset = false;

App *app = NULL;

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback() {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

char buffer[200];

WEBServer *webServer = NULL;

// This values  will depend on what the user configures
// on the  WifiManager on the first connection
char server[16] = "";
char log_server[30] = "";
char baseURL[30] = "";

void scan_networks() {

  char *buffer = new char[1024];
  int n = WiFi.scanNetworks();
  char *connected = NULL;

  if (n == 0) {
    Serial.println("no networks found");
  } else {
    sprintf(buffer, "networks found:\n");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found

      if (WiFi.SSID(i) == WiFi.SSID()) {
        connected = "*";
      } else {
        connected = " ";
      }

      sprintf(buffer + strlen(buffer), "%d [%s] %-32s dBM %d (%d%%)\n",
              i + 1,
              connected,
              WiFi.SSID(i).c_str(),
              WiFi.RSSI(i),
              dBmtoPercentage(WiFi.RSSI(i)));
    }
  }
  //webServer.send(200, "text/plain", buffer);
  delete[] buffer;
}

void wifi_signal() {
  sprintf(buffer, "%s [%d]\n", WiFi.SSID(), WiFi.RSSI());
  //webServer.send(200, "text/plain", buffer);
}


void reboot() {
  app->log("Board is going to reboot");
  //webServer.send(200, "text/plain", "OK\n");
  delay(2000);
  ESP.restart();
}

int dBmtoPercentage(int dBm) {
  int quality;
  if (dBm <= RSSI_MIN) {
    quality = 0;
  } else if (dBm >= RSSI_MAX) {
    quality = 100;
  } else {
    quality = 2 * (dBm + 100);
  }

  return quality;
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message +=  //webServer.uri();
    message += "\nMethod: ";
  //message += (webServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  //message += webServer.args();
  message += "\n";
  //for (uint8_t i = 0; i < webServer.args(); i++) {
  //  message += " " + webServer.argName(i) + ": " + webServer.arg(i) + "\n";
  //}
  //webServer.send(404, "text/plain", message);
}

void resetBoots() {
  app->resetBoots();
  //webServer.send(200, "text/plain", "boots resetted\n");
}

void readConfigFile() {
  Serial.println("mounting FS...");
  strcpy(server, "");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");
          strcpy(server, json["server"]);
        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
}

void play() {
  app->log("play");
}

void next() {
  app->log("next");
}

void previous() {
  app->log("previous");
}

void volumeup() {
  app->log("volume up");
}

void volumedown() {
  app->log("volume down");
}

void stop() {
  app->log("stop");
}

void startWebServer() {
  // Definir las rutas del servidor web
  //webServer.on("/", HTTP_GET, []() {
  //  webServer.send(200, "text/html", webpage);
  //});
}

void setup() {
  Serial.begin(115200);

  app = new App(BOARD_ID, server);

  /*
  app->wifiManager->resetSettings();
  Serial.println("Wifi reseted");
  delay(5000);
  return;
  */

  EEPROM.begin(EEPROM_SIZE);

  readConfigFile();
  WiFiManagerParameter pserver(SERVER_LABEL, "Server IP", server, 16);

  //set config save notify callback
  app->wifiManager->setSaveConfigCallback(saveConfigCallback);

  while (WiFi.status() != WL_CONNECTED) {
    app->startWiFiManager();
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &json = jsonBuffer.createObject();
    json["server"] = server;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  webServer = new WEBServer(app);
  webServer->start();

  // web page
  // startWebServer();

  // Other REST endpoints definition
  //webServerRouting();

  // Set not found response
  //webServer.onNotFound(handleNotFound);
  // Start server

  //webServer.begin();

  Serial.println("HTTP server started");

  unsigned short boots = app->incBoots();
  sprintf(buffer, "boot: %d", boots);
  app->log(buffer);
}

void resetWIFI() {
  sprintf(buffer, "WIFI networks will be reset and board rebooted NOW");
  app->log(buffer);
  //webServer.send(200, "text/plain", buffer);
  _resetWifi();
}

void _resetWifi() {
  app->log("reset WIFI networks");
  app->wifiManager->resetSettings();
  ESP.restart();
}

void loop() {
  app->attendTimers();
  webServer->handleClient();
}
