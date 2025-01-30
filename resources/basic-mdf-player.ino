#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define BUSY_PIN 10
#define LED 12

SoftwareSerial mySerial(5, 4, false);
DFRobotDFPlayerMini myDFPlayer;

void setup() {

    pinMode(BUSY_PIN, INPUT_PULLUP);
    pinMode(LED, OUTPUT);

    Serial.begin(115200);
    mySerial.begin(9600);

    Serial.println("\nDFPlayer Mini Test");

    if (!myDFPlayer.begin(mySerial)) {
        Serial.println("DFPlayer Mini not detected!");
        while (true);
    }

    Serial.println("Player detected!");
    unsigned int totalFiles = myDFPlayer.readFileCounts(); // Get the total number of MP3 files in the SD card

    Serial.print("Total files detected: ");
    Serial.println(totalFiles);

    myDFPlayer.volume(30);
    //myDFPlayer.play(1);
}

void loop() {
  int busyState = digitalRead(BUSY_PIN);
  Serial.println(busyState);
  delay(10000);
}
