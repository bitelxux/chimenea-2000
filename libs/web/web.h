#ifndef web_h
#define web_h

#include <string>

#include <ESP8266WebServer.h>
#include <DFRobotDFPlayerMini.h>

#include <btlx25.h>
#include <player.h>

class WEBServer {
    public:
        WEBServer(App* app);
        void start();
	    void handleClient();
        void registerPlayer(Player* player);
    private:
        Player* player = NULL;

        ESP8266WebServer* server = NULL;
        App* app = NULL;
        String getClientStrIP();
        void log(char* msg);
    	void configureEndPoints();
    	void handleRoot();
    	void handleNotFound();
    	void help();
    	void helloWorld();
    	void boardID();
    	void version();
    	void uptime();
    	void boots();
    	void resetBoots();
    	void reboot();
    	void resetWIFI();
    	void WIFISignal();
    	void scanNetworks();
    	void resetEEPROM();
        // Specific player endpoints
        void play();
        void next();
        void previous();
        void volumeup();
        void volumedown();
        void track();
        void readVolume();
        void stop();
        void chimenea();

	// Not used, served from LittleFS now
	// index.html at /data
        const char* webpage = R"=====(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Chimenea-2000</title>
        <style>
            body {
                font-family: Arial, sans-serif;
                text-align: center;
                margin: 0;
                padding: 0;
                background-color: #f4f4f9;
            }
            h1 {
                margin: 20px;
                font-size: 2rem;
                color: #333;
            }
            .button-container {
                display: grid;
                grid-template-columns: repeat(3, 1fr);
                gap: 10px;
                max-width: 300px;
                margin: 0 auto;
            }
            button {
                padding: 15px;
                font-size: 1.5rem;
                border: none;
                border-radius: 5px;
                cursor: pointer;
                background-color: #007bff;
                color: #fff;
                transition: background-color 0.3s;
            }
            button:hover {
                background-color: #0056b3;
            }
            button:active {
                background-color: #0056b3; /* Aseguramos que el color sea consistente mientras se presiona */
                transition: none; /* Opcional: elimina la animación en este estado */
            }
            .wide {
                grid-column: span 3;
            }
        </style>
        <script>
            // Esto asegura que el botón vuelva a su color original en dispositivos táctiles
            document.addEventListener('touchstart', function (event) {
                if (event.target.tagName === 'BUTTON') {
                    const button = event.target;
                    button.style.backgroundColor = '#0056b3'; // Color al presionar
                    setTimeout(() => {
                        button.style.backgroundColor = '#007bff'; // Color original
                    }, 400); // Retorno al color original tras 200ms
                }
            }, { passive: true });
        </script>
        </head>
        <body>
            <h1>Chimenea-2000</h1>
            <div class="button-container">
                <button onclick="sendCommand('previous')"><<</button>
                <button onclick="sendCommand('play')">▶</button>
                <button onclick="sendCommand('next')">>></button>
                <button class="wide" onclick="sendCommand('volumeup')">Volume ↑</button>
                <button class="wide" onclick="sendCommand('volumedown')">Volume ↓</button>
                <button class="wide" onclick="sendCommand('stop')">■ Stop</button>
            </div>
            <script>
                function sendCommand(command) {
                    fetch(`/${command}`)
                        .then(response => {
                            if (response.ok) {
                                console.log(`${command} command sent successfully`);
                            } else {
                                console.error(`Failed to send ${command} command`);
                            }
                        })
                        .catch(error => {
                            console.error(`Error: ${error}`);
                        });
                }
            </script>
        </body>
        </html>
        )=====";
};      

#endif
