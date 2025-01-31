# to install espota.py, git clone https://github.com/esp8266/Arduino.git
# and copy the tools directory to /home/<usr>/Arduino

name:=ESP8266-mp3player.ino
port:=/dev/ttyUSB1
ip:=192.168.1.191

all: clean build upload

build: $(name)
	bash -c ./prepare.sh
	arduino-cli compile --output-dir ./build --fqbn esp8266:esp8266:nodemcu  $(name)
upload:
	/home/$(USER)/Arduino/tools/espota.py -p 8266 -r -d -i $(ip) -f "build/$(name).bin"
upload_usb:
	arduino-cli upload --fqbn esp8266:esp8266:nodemcu -p $(port) .
clean:
	rm -rf build
