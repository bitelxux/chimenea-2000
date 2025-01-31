#include <string>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

#include "../config.h"
#include "btlx25.h"

class Player {
    public:
        Player(App* app);
        void begin(SoftwareSerial serial);
	void play(int track);
	void stop();
	void next();
	void previous();
	void volumeup();
	void volumedown();
    private:
        App* app = NULL;
	DFRobotDFPlayerMini dfPlayer;
        boolean bStopped = false;
	unsigned short volume = 30;
};      
