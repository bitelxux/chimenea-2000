#ifndef player_h
#define player_h

#include <string>

#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#include <btlx25.h>

class Player: public DFRobotDFPlayerMini {
    public:
        Player(App* app);
		void handle();
		void setStopped(bool value);
		bool getStopped();
		void play(int track=0);
		void stop();
		void next();
		void previous();
    private:
        App* app;
        boolean bStopped;
		unsigned int trackNumber;
		unsigned int totalFiles;
};      

#endif