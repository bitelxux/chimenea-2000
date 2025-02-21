#include <player.h>
#include "../../config.h"

SoftwareSerial serial(RX, TX, false);

Player::Player(App* app) {
    this->app = app;
    this->trackNumber = 1;
    this->totalFiles = 0;

    Serial.println("starting serial");
    serial.begin(9600);
    Serial.println("serial started");

    if (!this->begin(serial)) {
        this->app->log("DFPlayer Mini not detected! Rebooting board");
        while(true);
    }
    else {
        delay(1000);
        this->totalFiles = this->readFileCounts();
        char buffer[100];
        sprintf(buffer, "DPPlayer found [%d tracks]", this->totalFiles);
        this->app->log(buffer);
        this->volume(30);
        this->play(this->trackNumber);
    }
}

unsigned int Player::track() {
    return this->trackNumber;
}

void Player::play(int track) {
    track = (track == 0) ? this->trackNumber : track;
    //DFRobotDFPlayerMini::loop(track);
    this->loop(track);
    this->stopped = false;
    char buffer[50];
    sprintf(buffer, "Playing track %d", track);
    this->app->log(buffer);
}

void Player::stop() {
    this->stopped = true;
    DFRobotDFPlayerMini::stop();
    this->app->log("Stopping reproduction");
}

void Player::next() {
    // readCurrentFileNumber() is flacky
    // instead automanaging current track
    // and using play(track) instead of next() to ensure match
    if (this->trackNumber < this->totalFiles) {
        this->trackNumber++;
    }
    else
    {
        this->trackNumber = 1;
    }
    this->play(this->trackNumber);
}

void Player::previous() {
    // readCurrentFileNumber() is flacky
    // instead automanaging current track
    // and using play(track) instead of previous() to ensure match
    if (this->trackNumber > 1) {
        this->trackNumber--;
    }
    else
    {
        this->trackNumber = this->totalFiles;
    }
    this->play(this->trackNumber);
}

void Player::handle() {
  // on this module loop() doesnt work
  // implementing it
  // int playing = !digitalRead(BUSY_PIN);
  // defective module. BUSY pin works randomly
  // reading status instead

  //int playing = !digitalRead(BUSY_PIN);

  static unsigned long lastTime = millis();
  unsigned long currentMillis = millis();

  if (currentMillis - lastTime < 500 ) {
     return;
  }

  int playing = this->readState();
  lastTime = currentMillis;

  if (!this->stopped && playing == 512) {
     digitalWrite(GREEN_LED, playing);
     this->play(this->trackNumber);
     lastTime = currentMillis + 2000; // to give status time to be set
  }

}




