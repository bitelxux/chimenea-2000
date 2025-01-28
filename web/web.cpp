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
    this->server->onNotFound([this]() { this->handleNotFound(); });
    this->server->begin();
}

void WEBServer::handleClient() {
    this->server->handleClient();
}

void WEBServer::handleNotFound() {
  String message = "Endpoint not nound\n\n";
  message += "URI: ";
  message +=  this->server->uri();
  message += "\nMethod: ";
  message += (this->server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += this->server->args();
  message += "\n";
  for (uint8_t i = 0; i < this->server->args(); i++) {
    message += " " + this->server->argName(i) + ": " + this->server->arg(i) + "\n";
  }
  this->server->send(404, "text/plain", message);
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
  this->server->on(F("/boardID"), HTTP_GET, [this]() { this->boardID(); });
  this->server->on(F("/version"), HTTP_GET, [this]() { this->version(); });
  this->server->on(F("/uptime"), HTTP_GET, [this]() { this->uptime(); });
  this->server->on(F("/boots"), HTTP_GET, [this]() { this->boots(); });
  this->server->on(F("/resetBoots"), HTTP_GET, [this]() { this->resetBoots(); });
  this->server->on(F("/reboot"), HTTP_GET, [this]() { this->reboot(); });
  this->server->on(F("/resetWIFI"), HTTP_GET, [this]() { this->resetWIFI(); });
  this->server->on(F("/WIFISignal"), HTTP_GET, [this]() { this->WIFISignal(); });
  this->server->on(F("/scanNetworks"), HTTP_GET, [this]() { this->scanNetworks(); });
  this->server->on(F("/resetEEPROM"), HTTP_GET, [this]() { this->resetEEPROM(); });
}

std::string WEBServer::getClientStrIP() {
  IPAddress clientIP = this->server->client().remoteIP();
  char ipStr[16];

  sprintf(ipStr, "%d.%d.%d.%d", clientIP[0], clientIP[1], clientIP[2], clientIP[3]);
  return std::string(ipStr);
}

void WEBServer::log(char* msg) {
  this->app->log(msg);
}

void WEBServer::resetEEPROM() {
  char buffer[50];
  this->log("called /resetEEPROM endpoint");
  int t = this->app->resetEEPROM(0, EEPROM_SIZE);
  sprintf(buffer, "EEPROM deleted in %d milliseconds", t);
  this->server->send(200, "test/plain", buffer);
}

void WEBServer::resetWIFI() {
  this->log("called /resetWIFI endpoint");
}

void WEBServer::WIFISignal() {
  this->log("called /WIFISignal endpoint");
}

void WEBServer::scanNetworks() {
  this->log("called /scanNetworks endpoint");
}

void WEBServer::reboot() {
  this->log("called /reboot endpoint");
  this->log("Board is going to reboot");
  delay(2000);
  ESP.restart();
}

void WEBServer::resetBoots() {
  this->log("called /resetBoots endpoint");
  this->app->resetBoots();
  this->server->send(200, "text/plain", "boots resetted\n");
}

void WEBServer::boots() {
  this->log("called /boots endpoint");
  char buffer[20];
  sprintf(buffer, "%d\n", this->app->readBoots());
  this->server->send(200, "text/plain", buffer);
}

void WEBServer::uptime() {
  this->log("called /uptime endpoint");
  char buffer[20];
  sprintf(buffer, "%s\n", this->app->millis_to_human(millis()));
  this->server->send(200, "text/plain", buffer);
}

void WEBServer::version() {
  this->log("called /version endpoint");
  char buffer[20];
  sprintf(buffer, "%s\n", VERSION);
  this->server->send(200, "text/plain", buffer);
}

void WEBServer::boardID() {
  this->log("called /boardID endpoint");
  char buffer[20];
  sprintf(buffer, "%s\n", BOARD_ID);
  this->server->send(200, "text/plain", buffer);
}

void WEBServer::helloWorld() {
  this->log("called /helloWorld endpoint");
  this->server->send(200, "text/json", "{\"name\": \"Hello world\"}");
}

void WEBServer::help() {

  this->log("called /help endpoint");

  char buffer[1024];
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
                  "resetEEPROM: clears all info in EEPROM\n"
                  "resetWIFI: deletes WIFI known networks\n"
                  "WIFISignal: WIFI RSSI\n"
                  "scanNetworks: info about WIFI networks\n",
          VERSION);
  this->server->send(200, "text/plain", buffer);
}
