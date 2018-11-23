# Makefile

SDK = ${HOME}/local/espressif/ESP8266_NONOS_SDK

CC = xtensa-lx106-elf-gcc
CFLAGS = -I${SDK}/include -I. -mlongcalls -DSPI_FLASH_SIZE_MAP=6
LDLIBS = -nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -lcrypto -llwip -lpp -lphy -lc -Wl,--end-group -lgcc
LDFLAGS = -L${SDK}/lib -T${SDK}/ld/eagle.app.v6.ld

blinky-0x00000.bin: blinky
	esptool.py elf2image $^

blinky: blinky.o

blinky.o: user_config.h blinky.c

flash: blinky-0x00000.bin
	esptool.py write_flash 0 blinky-0x00000.bin 0x10000 blinky-0x10000.bin

clean:
	rm -f blinky blinky.o blinky-0x00000.bin blinky-0x10000.bin
