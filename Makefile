# to install espota.py, git clone https://github.com/esp8266/Arduino.git
# and copy the tools directory to /home/<usr>/Arduino
#

# USB_PORT and IP must be environment variables (export them)

name:=ESP8266-mp3player.ino
board:=generic
#board:=nodemcu
fs_filename:=filesystem.img

#.arduino is created at /home/cnn by the IDE. It already contains esptool and mkspiffs that
#we can point ti
mkspiffs:=/home/cnn/.arduino15/packages/esp8266/tools/mkspiffs/3.1.0-gcc10.3-e5f9fec/mkspiffs
esptool=python3 /home/cnn/.arduino15/packages/esp8266/hardware/esp8266/3.1.1/tools/esptool/esptool.py

all: clean build upload
usb: clean build upload_usb

build: $(name)
	bash -c ./prepare.sh
	arduino-cli compile --output-dir ./build --fqbn esp8266:esp8266:$(board)  $(name)

upload_fs:
	# This does not work yet. Workaround to upload from arduino IDE
	$(mkspiffs) -c ./data -b 4096 -p 256 -s 65536 $(fs_filename)
	$(esptool) --chip esp8266 --port $(USB_PORT) write_flash --verify 0x000EB000 $(fs_filename)
	rm $(fs_filename)

upload:
	/home/$(USER)/Arduino/tools/espota.py -p 8266 -r -d -i $(IP) -f "build/$(name).bin"

upload_usb:
	# erase_flash will delete wifi configuration
	#esptool.py --port $(USB_PORT) erase_flash
	killall -TERM screen || true
	arduino-cli upload --fqbn esp8266:esp8266:$(board) -p $(USB_PORT) -t .
clean:
	rm -rf build
