# to install espota.py, git clone https://github.com/esp8266/Arduino.git
# and copy the tools directory to /home/<usr>/Arduino
#

# USB_PORT and IP must be environment variables (export them)

name:=ESP8266-mp3player.ino
board:=generic
#board:=nodemcu

all: clean build upload
usb: clean build upload_usb

build: $(name)
	bash -c ./prepare.sh
	arduino-cli compile --output-dir ./build --fqbn esp8266:esp8266:$(board)  $(name)
upload:
	/home/$(USER)/Arduino/tools/espota.py -p 8266 -r -d -i $(IP) -f "build/$(name).bin"
upload_usb:
	# erase_flash will delete wifi configuration
	#esptool.py --port $(USB_PORT) erase_flash
	arduino-cli upload --fqbn esp8266:esp8266:$(board) -p $(USB_PORT) -t .
clean:
	rm -rf build
