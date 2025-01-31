#include "player.h"

Player::Player(App* app) {
    this->app = app;
}

void Player::begin(SoftwareSerial serial) {
    if (!this->dfPlayer.begin(serial)) {
        this->app->log("Player not found");
    }

    this->dfPlayer.volume(this->volume);
}

void Player::play(int track) {
    this->dfPlayer.play(track);
}

void Player::stop() {
    this->dfPlayer.stop();
}

void Player::next() {
    this->dfPlayer.next();
}

void Player::previous() {
    this->dfPlayer.previous();
}

void Player::volumeup() {
    this->dfPlayer.volumeUp();
}

void Player::volumedown() {
    this->dfPlayer.volumeDown();
}

