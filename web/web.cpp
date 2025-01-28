#include <string>

#include "web.h"
#include "../config.h"

WEBServer::WEBServer(App* app) {
    this->server = new ESP8266WebServer(80);
    this->app = app;
}

void WEBServer::start() {
    this->server->on("/", HTTP_GET, [this]() {
        this->server->send(200, "text/html", this->webpage);
    });

    this->configureEndPoints();
    this->server->begin();
}

void WEBServer::handleClient() {
    this->server->handleClient();
}

void WEBServer::configureEndPoints() {
  // specific webpoints
  //webServer.on(F("/next"), HTTP_GET, next);
  //webServer.on(F("/previous"), HTTP_GET, previous);
  //webServer.on(F("/play"), HTTP_GET, play);
  //webServer.on(F("/volumeup"), HTTP_GET, volumeup);
  //webServer.on(F("/volumedown"), HTTP_GET, volumedown);
  //webServer.on(F("/stop"), HTTP_GET, stop);
  // generic endpoints
  this->server->on(F("/help"), HTTP_GET, [this]() { this->help(); });
  this->server->on(F("/helloWorld"), HTTP_GET, [this]() { this->helloWorld(); });
  //webServer.on(F("/boardID"), HTTP_GET, boardID);
  //webServer.on(F("/version"), HTTP_GET, version);
  //webServer.on(F("/uptime"), HTTP_GET, uptime);
  //webServer.on(F("/boots"), HTTP_GET, boots);
  //webServer.on(F("/resetBoots"), HTTP_GET, resetBoots);
  //webServer.on(F("/reboot"), HTTP_GET, reboot);
  //webServer.on(F("/resetWIFI"), HTTP_GET, resetWIFI);
  //webServer.on(F("/WIFISignal"), HTTP_GET, wifi_signal);
  //webServer.on(F("/scanNetworks"), HTTP_GET, scan_networks);
}

std::string WEBServer::getClientStrIP() {
  IPAddress clientIP = this->server->client().remoteIP();
  char ipStr[16];

  sprintf(ipStr, "%d.%d.%d.%d", clientIP[0], clientIP[1], clientIP[2], clientIP[3]);
  return std::string(ipStr);
}

void WEBServer::log(char* msg) {
  char* buffer = new char[1024];
  std::string ipStr = this->getClientStrIP();

  sprintf(buffer, "[%s] %s", ipStr.c_str(), msg);
  this->app->log(buffer);
  delete[] buffer;
}

void WEBServer::helloWorld() {
  this->log("called /helloWorld endpoint");
  this->server->send(200, "text/json", "{\"name\": \"Hello world\"}");
}

void WEBServer::help() {

  this->log("called /help endpoint");

  char* buffer = new char[1024];
  sprintf(buffer, "mp3 Player) %s\n"
                  "----------------------------------------------------------------\n"
                  "\n"
                  "help: API help\n"
                  "helloWorld: json payload example\n"
                  "boardID: gets board's ID\n"
                  "version: build version\n"
                  "uptime: gets uptime\n"
                  "boots: number of boots so long\n"
                  "resetBoots: resets number of boots to 0\n"
                  "reboot: reboots the board\n"
                  "resetEEPROM: clears all info in EEPROM, including local readings.\n"
                  "resetWIFI: deletes WIFI known networks\n"
                  "WIFISignal: WIFI RSSI\n"
                  "scanNetworks: info about WIFI networks\n",
          VERSION);
  this->server->send(200, "text/plain", buffer);
  delete[] buffer; 
}
