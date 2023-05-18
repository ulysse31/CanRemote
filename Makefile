# Uncomment lines below if you have problems with $PATH
#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)

all:
	pio -f  run --upload-port /dev/ttyACM2

upload:
	pio -f  run --target upload --upload-port /dev/ttyACM2

clean:
	pio -f  run --target clean

program:
	pio -f  run --target program

uploadfs:
	pio -f  run --target uploadfs --upload-port /dev/ttyACM2

update:
	pio -f  update --upload-port /dev/ttyACM2

monitor:
	pio device monitor --port /dev/ttyACM2
