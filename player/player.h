#include <string>

#include <ESP8266WebServer.h>

#include "btlx25.h"

class Player {
    public:
        Player(App* app);
        void begin(SofwareSerial serial);
	void play(int track);
	void stop();
	void next();
	void previous();
	void volumeup();
	void volumedown();
    private:
        App* app = NULL;
        boolean bStopped = false;
	unsigned short volume = 30;
};      
