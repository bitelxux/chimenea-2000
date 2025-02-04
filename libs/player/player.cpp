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
        //this->app->log("DFPlayer Mini not detected! Rebooting board");
        Serial.println("DFPlayer Mini not detected! Rebooting board");
        while(true);
    }
    else {
        delay(1000);
        this->totalFiles = this->readFileCounts();
        char buffer[100];
        sprintf(buffer, "DPPlayer found [%d tracks]", this->totalFiles);
	Serial.println(buffer);
        //this->app->log(buffer);
        this->volume(10);
        this->play(this->trackNumber);
    }
}

unsigned int Player::track() {
    return this->trackNumber;
}

void Player::play(int track) {
    track = (track == 0) ? this->trackNumber : track;
    DFRobotDFPlayerMini::loop(track);
    char buffer[50];
    sprintf(buffer, "Playing track %d", track);
    this->app->log(buffer);
}

void Player::stop() {
    DFRobotDFPlayerMini::stop();
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
  int playing = !digitalRead(BUSY_PIN);
  digitalWrite(LED, digitalRead(BUSY_PIN));
}
