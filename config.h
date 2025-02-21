// This is for each variable to use it's real size when stored
// in the EEPROM
//
//

#pragma pack(push, 1)

#define BOARD_ID "chimenea-2000"
#define VERSION "20250221.669"

//EEPROM
#define EEPROM_SIZE 1024

#define RSSI_MAX -50   // maximum strength of signal in dBm
#define RSSI_MIN -100  // minimum strength of signal in dBm

// WiFiManager parameteres
#define SERVER_LABEL "SERVER_IP"

// BOARD
#define RESET 13
#define GREEN_LED 16
#define BLUE_LED 14
#define BUSY_PIN 12

// to connect FDF player with SoftwareSerial
#define RX 4
#define TX 5

