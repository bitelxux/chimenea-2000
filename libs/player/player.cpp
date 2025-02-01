#include <player.h>
#include "../../config.h"

SoftwareSerial serial(RX, TX, false);

Player::Player(App* app) {
    this->app = app;
    this->bStopped = false;
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
        this->totalFiles = this->readFileCounts();
        char buffer[100];
        sprintf(buffer, "DPPlayer found [%d tracks]", this->totalFiles);
        this->app->log(buffer);
        this->volume(30);
        this->loop(this->trackNumber);
    }
}

void Player::play(int track) {
    track = (track == 0) ? this->trackNumber : track;
    DFRobotDFPlayerMini::loop(track);
    char buffer[50];
    sprintf(buffer, "Playing track %d", track);
    this->app->log(buffer);
    this->bStopped = false;
}

void Player::stop() {
    DFRobotDFPlayerMini::stop();
    this->bStopped = true;
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

void Player::setStopped(bool value) {
    this->bStopped = value;
}

bool Player::getStopped() {
    return this->bStopped;
}

void Player::handle() {
  int playing = !digitalRead(BUSY_PIN);
  digitalWrite(LED, digitalRead(BUSY_PIN));
}
