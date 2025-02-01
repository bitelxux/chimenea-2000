#include <player.h>
#include "../../config.h"

SoftwareSerial serial(4, 5, false);

Player::Player(App* app) {
    this->app = app;
    this->bStopped = false;
    //this->app->log("This is the player");

    Serial.println("starting serial");
    serial.begin(9600);
    Serial.println("serial started");

    if (!this->begin(serial)) {
        Serial.println("DFPlayer Mini not detected! Rebooting board");
        while(true);
    }
    else {
        Serial.println("DFPlayer found!");
        //this->volume(30);
        //this->play(1);
    }
}
