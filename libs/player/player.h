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
    private:
        App* app;
        boolean bStopped;
	    unsigned short volume;
};      

#endif