
#BUILD_PATH=/var/folders/sd/khh05h_j2d3f99f29dpwbk8w0000gn/T/arduino_build_558357
#BUILD_CACHE=/var/folders/sd/khh05h_j2d3f99f29dpwbk8w0000gn/T/arduino_cache_780914

BUILD_PATH=$(abspath ./.build)
BUILD_CACHE=$(abspath ./.cache)

FQBN=arduino:avr:leonardo

MCU=atmega32u4
IDE=10805

PORT ?= /dev/cu.usbmodem1421

PROJECT_ROOT=$(CURDIR)
PROJECT_NAME=$(notdir $(PROJECT_ROOT))

# ARDUINO_BUILDER=/Applications/Arduino.app/Contents/Java/arduino-builder
ARDUINO_BUILDER=/Users/max_romanov/projects/arduino-builder/arduino-builder

AVR_GXX="/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2/bin/avr-g++"
AVR_GCC="/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2/bin/avr-gcc"

CPPFLAGS = -std=c++11 -xc++ -MMD -lncurses

.PHONY: all compile prefs upload dirs

all: compile

patch_mate: patch_mate.ino
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LOADLIBES) $(LDLIBS) -o $@ $<

-include patch_mate.d

$(BUILD_PATH)/$(PROJECT_NAME).ino.hex: compile

dirs: $(BUILD_PATH) $(BUILD_CACHE)

$(BUILD_PATH) $(BUILD_CACHE):
	mkdir -p $@

prefs: dirs
	$(ARDUINO_BUILDER) \
    -dump-prefs \
    -logger=human \
    -hardware /Applications/Arduino.app/Contents/Java/hardware \
    -hardware /Volumes/data/home/max_romanov/Library/Arduino15/packages \
    -tools /Applications/Arduino.app/Contents/Java/tools-builder \
    -tools /Applications/Arduino.app/Contents/Java/hardware/tools/avr \
    -tools /Volumes/data/home/max_romanov/Library/Arduino15/packages \
    -built-in-libraries /Applications/Arduino.app/Contents/Java/libraries \
    -libraries /Volumes/data/home/max_romanov/Documents/Arduino/libraries \
    -fqbn=$(FQBN) \
    -ide-version=$(IDE) \
    -build-path $(BUILD_PATH) \
    -warnings=all \
    -build-cache $(BUILD_CACHE) \
    -prefs=build.warn_data_percentage=75 \
    -prefs=runtime.tools.avrdude.path=/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avrdude/6.3.0-arduino14 \
    -prefs=runtime.tools.arduinoOTA.path=/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/arduinoOTA/1.2.1 \
    -prefs=runtime.tools.avr-gcc.path=/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2 \
    -verbose $(PROJECT_ROOT)/$(PROJECT_NAME).ino \

compile: dirs
	$(ARDUINO_BUILDER) \
    -compile \
    -logger=human \
    -hardware /Applications/Arduino.app/Contents/Java/hardware \
    -hardware /Volumes/data/home/max_romanov/Library/Arduino15/packages \
    -tools /Applications/Arduino.app/Contents/Java/tools-builder \
    -tools /Applications/Arduino.app/Contents/Java/hardware/tools/avr \
    -tools /Volumes/data/home/max_romanov/Library/Arduino15/packages \
    -built-in-libraries /Applications/Arduino.app/Contents/Java/libraries \
    -libraries /Volumes/data/home/max_romanov/Documents/Arduino/libraries \
    -fqbn=$(FQBN) \
    -ide-version=$(IDE) \
    -build-path $(BUILD_PATH) \
    -warnings=all \
    -build-cache $(BUILD_CACHE) \
    -prefs=build.warn_data_percentage=75 \
    -prefs=runtime.tools.avrdude.path=/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avrdude/6.3.0-arduino14 \
    -prefs=runtime.tools.arduinoOTA.path=/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/arduinoOTA/1.2.1 \
    -prefs=runtime.tools.avr-gcc.path=/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2 \
    -verbose $(PROJECT_ROOT)/$(PROJECT_NAME).ino \

csc:
	$(AVR_GXX) \
    -xc++ \
    -c \
    -g \
    -Os \
    -Wall \
    -Wextra \
    -std=gnu++11 \
    -fpermissive \
    -fno-exceptions \
    -ffunction-sections \
    -fdata-sections \
    -fno-threadsafe-statics \
    -Wno-error=narrowing \
    -MMD \
    -flto \
    -mmcu=$(MCU) \
    -DF_CPU=16000000L \
    -DARDUINO=$(IDE) \
    -DARDUINO_AVR_LEONARDO \
    -DARDUINO_ARCH_AVR \
    -DUSB_VID=0x2341 \
    -DUSB_PID=0x8036 \
    '-DUSB_MANUFACTURER="Unknown"' \
    '-DUSB_PRODUCT="Arduino Leonardo"' \
    "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/cores/arduino" \
    "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/variants/leonardo" \
    "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/libraries/EEPROM/src" \
    "-I/Applications/Arduino.app/Contents/Java/libraries/LiquidCrystal/src" \
    "$(PROJECT_NAME).ino" \
    -o "$(BUILD_PATH)/sketch/$(PROJECT_NAME).ino.o" \

-include "$(BUILD_PATH)/sketch/$(PROJECT_NAME).ino.d"

# Using board 'leonardo' from platform in folder: /Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23
# Using core 'arduino' from platform in folder: /Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23
# Detecting libraries used...
#
# "/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2/bin/avr-g++"
#     -c
#     -g
#     -Os
#     -w
#     -std=gnu++11
#     -fpermissive
#     -fno-exceptions
#     -ffunction-sections
#     -fdata-sections
#     -fno-threadsafe-statics
#     -Wno-error=narrowing
#     -flto
#     -w
#     -x c++
#     -E
#     -CC
#     -mmcu=$(MCU)
#     -DF_CPU=16000000L
#     -DARDUINO=$(IDE)
#     -DARDUINO_AVR_LEONARDO
#     -DARDUINO_ARCH_AVR
#     -DUSB_VID=0x2341
#     -DUSB_PID=0x8036
#     '-DUSB_MANUFACTURER="Unknown"'
#     '-DUSB_PRODUCT="Arduino Leonardo"'
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/cores/arduino"
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/variants/leonardo"
#     "$(BUILD_PATH)/sketch/$(PROJECT_NAME).ino.cpp"
#     -o "/dev/null"
#
# "/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2/bin/avr-g++"
#     -c
#     -g
#     -Os
#     -w
#     -std=gnu++11
#     -fpermissive
#     -fno-exceptions
#     -ffunction-sections
#     -fdata-sections
#     -fno-threadsafe-statics
#     -Wno-error=narrowing
#     -flto
#     -w
#     -x c++
#     -E
#     -CC
#     -mmcu=$(MCU)
#     -DF_CPU=16000000L
#     -DARDUINO=$(IDE)
#     -DARDUINO_AVR_LEONARDO
#     -DARDUINO_ARCH_AVR
#     -DUSB_VID=0x2341
#     -DUSB_PID=0x8036
#     '-DUSB_MANUFACTURER="Unknown"'
#     '-DUSB_PRODUCT="Arduino Leonardo"'
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/cores/arduino"
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/variants/leonardo"
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/libraries/EEPROM/src"
#     "$(BUILD_PATH)/sketch/$(PROJECT_NAME).ino.cpp"
#     -o "/dev/null"
#
# "/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2/bin/avr-g++"
#     -c
#     -g
#     -Os
#     -w
#     -std=gnu++11
#     -fpermissive
#     -fno-exceptions
#     -ffunction-sections
#     -fdata-sections
#     -fno-threadsafe-statics
#     -Wno-error=narrowing
#     -flto
#     -w
#     -x c++
#     -E
#     -CC
#     -mmcu=$(MCU)
#     -DF_CPU=16000000L
#     -DARDUINO=$(IDE)
#     -DARDUINO_AVR_LEONARDO
#     -DARDUINO_ARCH_AVR
#     -DUSB_VID=0x2341
#     -DUSB_PID=0x8036
#     '-DUSB_MANUFACTURER="Unknown"'
#     '-DUSB_PRODUCT="Arduino Leonardo"'
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/cores/arduino"
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/variants/leonardo"
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/libraries/EEPROM/src"
#     "-I/Applications/Arduino.app/Contents/Java/libraries/LiquidCrystal/src"
#     "$(BUILD_PATH)/sketch/$(PROJECT_NAME).ino.cpp"
#     -o "/dev/null"
#
# Using cached library dependencies for file: /Applications/Arduino.app/Contents/Java/libraries/LiquidCrystal/src/LiquidCrystal.cpp
# Generating function prototypes...
#
# "/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2/bin/avr-g++"
#     -c
#     -g
#     -Os
#     -w
#     -std=gnu++11
#     -fpermissive
#     -fno-exceptions
#     -ffunction-sections
#     -fdata-sections
#     -fno-threadsafe-statics
#     -Wno-error=narrowing
#     -flto
#     -w
#     -x c++
#     -E
#     -CC
#     -mmcu=$(MCU)
#     -DF_CPU=16000000L
#     -DARDUINO=$(IDE)
#     -DARDUINO_AVR_LEONARDO
#     -DARDUINO_ARCH_AVR
#     -DUSB_VID=0x2341
#     -DUSB_PID=0x8036
#     '-DUSB_MANUFACTURER="Unknown"'
#     '-DUSB_PRODUCT="Arduino Leonardo"'
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/cores/arduino"
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/variants/leonardo"
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/libraries/EEPROM/src"
#     "-I/Applications/Arduino.app/Contents/Java/libraries/LiquidCrystal/src"
#     "$(BUILD_PATH)/sketch/$(PROJECT_NAME).ino.cpp"
#     -o "$(BUILD_PATH)/preproc/ctags_target_for_gcc_minus_e.cpp"
#
# "/Applications/Arduino.app/Contents/Java/tools-builder/ctags/5.8-arduino11/ctags"
#     -u
#     --language-force=c++
#     -f
#     -
#     --c++-kinds=svpf
#     --fields=KSTtzns
#     --line-directives "$(BUILD_PATH)/preproc/ctags_target_for_gcc_minus_e.cpp"
#
# Compiling sketch...
#
# "/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2/bin/avr-g++"
#     -c
#     -g
#     -Os
#     -Wall
#     -Wextra
#     -std=gnu++11
#     -fpermissive
#     -fno-exceptions
#     -ffunction-sections
#     -fdata-sections
#     -fno-threadsafe-statics
#     -Wno-error=narrowing
#     -MMD
#     -flto
#     -mmcu=$(MCU)
#     -DF_CPU=16000000L
#     -DARDUINO=$(IDE)
#     -DARDUINO_AVR_LEONARDO
#     -DARDUINO_ARCH_AVR
#     -DUSB_VID=0x2341
#     -DUSB_PID=0x8036
#     '-DUSB_MANUFACTURER="Unknown"'
#     '-DUSB_PRODUCT="Arduino Leonardo"'
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/cores/arduino"
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/variants/leonardo"
#     "-I/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/libraries/EEPROM/src"
#     "-I/Applications/Arduino.app/Contents/Java/libraries/LiquidCrystal/src"
#     "$(BUILD_PATH)/sketch/$(PROJECT_NAME).ino.cpp"
#     -o "$(BUILD_PATH)/sketch/$(PROJECT_NAME).ino.cpp.o"
#
# Compiling libraries...
# Compiling library "EEPROM"
# Compiling library "LiquidCrystal"
# Using previously compiled file: $(BUILD_PATH)/libraries/LiquidCrystal/LiquidCrystal.cpp.o
# Compiling core...
# Using precompiled core
# Linking everything together...
#
# "/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2/bin/avr-gcc"
#     -Wall
#     -Wextra
#     -Os
#     -g
#     -flto
#     -fuse-linker-plugin
#     -Wl,--gc-sections
#     -mmcu=$(MCU)
#     -o "$(BUILD_PATH)/$(PROJECT_NAME).ino.elf"
#     "$(BUILD_PATH)/sketch/$(PROJECT_NAME).ino.cpp.o"
#     "$(BUILD_PATH)/libraries/LiquidCrystal/LiquidCrystal.cpp.o"
#     "$(BUILD_CACHE)/core/core_arduino_avr_leonardo_0bef2af4b475cbeeba654fc4d8263ddd.a"
#     "-L$(BUILD_PATH)"
#     -lm
#
# "/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2/bin/avr-objcopy"
#     -O ihex
#     -j .eeprom
#     --set-section-flags=.eeprom=alloc,load
#     --no-change-warnings
#     --change-section-lma .eeprom=0
#     "$(BUILD_PATH)/$(PROJECT_NAME).ino.elf"
#     "$(BUILD_PATH)/$(PROJECT_NAME).ino.eep"
#
# "/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avr-gcc/5.4.0-atmel3.6.1-arduino2/bin/avr-objcopy"
#     -O ihex
#     -R .eeprom
#     "$(BUILD_PATH)/$(PROJECT_NAME).ino.elf"
#     "$(BUILD_PATH)/$(PROJECT_NAME).ino.hex"
#
# Using library EEPROM at version 2.0 in folder: /Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/hardware/avr/1.6.23/libraries/EEPROM
# Using library LiquidCrystal at version 1.0.7 in folder: /Applications/Arduino.app/Contents/Java/libraries/LiquidCrystal
# Sketch uses 22344 bytes (77%) of program storage space. Maximum is 28672 bytes.
# Global variables use 1923 bytes (75%) of dynamic memory, leaving 637 bytes for local variables. Maximum is 2560 bytes.
# Low memory available, stability problems may occur.
#
#
# Forcing reset using 1200bps open/close on port /dev/cu.usbmodem1421
# PORTS {/dev/cu.Bluetooth-Incoming-Port, /dev/cu.C3530-BluetoothSerialPo, /dev/cu.HC-06-DevB, /dev/cu.usbmodem1411, /dev/cu.usbmodem1421, /dev/cu.YAS-152Yamaha-SPPDev, /dev/tty.Bluetooth-Incoming-Port, /dev/tty.C3530-BluetoothSerialPo, /dev/tty.HC-06-DevB, /dev/tty.usbmodem1411, /dev/tty.usbmodem1421, /dev/tty.YAS-152Yamaha-SPPDev, } / {/dev/cu.Bluetooth-Incoming-Port, /dev/cu.C3530-BluetoothSerialPo, /dev/cu.HC-06-DevB, /dev/cu.usbmodem1411, /dev/cu.usbmodem1421, /dev/cu.YAS-152Yamaha-SPPDev, /dev/tty.Bluetooth-Incoming-Port, /dev/tty.C3530-BluetoothSerialPo, /dev/tty.HC-06-DevB, /dev/tty.usbmodem1411, /dev/tty.usbmodem1421, /dev/tty.YAS-152Yamaha-SPPDev, } => {}
# PORTS {/dev/cu.Bluetooth-Incoming-Port, /dev/cu.C3530-BluetoothSerialPo, /dev/cu.HC-06-DevB, /dev/cu.usbmodem1411, /dev/cu.usbmodem1421, /dev/cu.YAS-152Yamaha-SPPDev, /dev/tty.Bluetooth-Incoming-Port, /dev/tty.C3530-BluetoothSerialPo, /dev/tty.HC-06-DevB, /dev/tty.usbmodem1411, /dev/tty.usbmodem1421, /dev/tty.YAS-152Yamaha-SPPDev, } / {/dev/cu.Bluetooth-Incoming-Port, /dev/cu.C3530-BluetoothSerialPo, /dev/cu.HC-06-DevB, /dev/cu.usbmodem1411, /dev/cu.YAS-152Yamaha-SPPDev, /dev/tty.Bluetooth-Incoming-Port, /dev/tty.C3530-BluetoothSerialPo, /dev/tty.HC-06-DevB, /dev/tty.usbmodem1411, /dev/tty.YAS-152Yamaha-SPPDev, } => {}
# PORTS {/dev/cu.Bluetooth-Incoming-Port, /dev/cu.C3530-BluetoothSerialPo, /dev/cu.HC-06-DevB, /dev/cu.usbmodem1411, /dev/cu.YAS-152Yamaha-SPPDev, /dev/tty.Bluetooth-Incoming-Port, /dev/tty.C3530-BluetoothSerialPo, /dev/tty.HC-06-DevB, /dev/tty.usbmodem1411, /dev/tty.YAS-152Yamaha-SPPDev, } / {/dev/cu.Bluetooth-Incoming-Port, /dev/cu.C3530-BluetoothSerialPo, /dev/cu.HC-06-DevB, /dev/cu.usbmodem1411, /dev/cu.usbmodem1421, /dev/cu.YAS-152Yamaha-SPPDev, /dev/tty.Bluetooth-Incoming-Port, /dev/tty.C3530-BluetoothSerialPo, /dev/tty.HC-06-DevB, /dev/tty.usbmodem1411, /dev/tty.usbmodem1421, /dev/tty.YAS-152Yamaha-SPPDev, } => {/dev/cu.usbmodem1421, /dev/tty.usbmodem1421, }
# Found upload port: /dev/cu.usbmodem1421
#

upload: $(BUILD_PATH)/$(PROJECT_NAME).ino.hex
	/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avrdude/6.3.0-arduino14/bin/avrdude \
    -C/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avrdude/6.3.0-arduino14/etc/avrdude.conf \
    -v \
    -p$(MCU) \
    -cavr109 \
    -P$(PORT) \
    -b57600 \
    -D \
    -Uflash:w:$(BUILD_PATH)/$(PROJECT_NAME).ino.hex:i \

# avrdude: Version 6.3-20171130
#          Copyright (c) 2000-2005 Brian Dean, http://www.bdmicro.com/
#          Copyright (c) 2007-2014 Joerg Wunsch
#
#          System wide configuration file is "/Volumes/data/home/max_romanov/Library/Arduino15/packages/arduino/tools/avrdude/6.3.0-arduino14/etc/avrdude.conf"
#          User configuration file is "/Users/max_romanov/.avrduderc"
#          User configuration file does not exist or is not a regular file, skipping
#
#          Using Port                    : /dev/cu.usbmodem1421
#          Using Programmer              : avr109
#          Overriding Baud Rate          : 57600
#          AVR Part                      : ATmega32U4
#          Chip Erase delay              : 9000 us
#          PAGEL                         : PD7
#          BS2                           : PA0
#          RESET disposition             : dedicated
#          RETRY pulse                   : SCK
#          serial program mode           : yes
#          parallel program mode         : yes
#          Timeout                       : 200
#          StabDelay                     : 100
#          CmdexeDelay                   : 25
#          SyncLoops                     : 32
#          ByteDelay                     : 0
#          PollIndex                     : 3
#          PollValue                     : 0x53
#          Memory Detail                 :
#
#                                   Block Poll               Page                       Polled
#            Memory Type Mode Delay Size  Indx Paged  Size   Size #Pages MinW  MaxW   ReadBack
#            ----------- ---- ----- ----- ---- ------ ------ ---- ------ ----- ----- ---------
#            eeprom        65    20     4    0 no       1024    4      0  9000  9000 0x00 0x00
#            flash         65     6   128    0 yes     32768  128    256  4500  4500 0x00 0x00
#            lfuse          0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
#            hfuse          0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
#            efuse          0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
#            lock           0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
#            calibration    0     0     0    0 no          1    0      0     0     0 0x00 0x00
#            signature      0     0     0    0 no          3    0      0     0     0 0x00 0x00
#
#          Programmer Type : butterfly
#          Description     : Atmel AppNote AVR109 Boot Loader
#
# Connecting to programmer: .
# Found programmer: Id = "CATERIN"; type = S
#     Software Version = 1.0; No Hardware Version given.
# Programmer supports auto addr increment.
# Programmer supports buffered memory access with buffersize=128 bytes.
#
# Programmer supports the following devices:
#     Device code: 0x44
#
# avrdude: devcode selected: 0x44
# avrdude: AVR device initialized and ready to accept instructions
#
# Reading | ################################################## | 100% 0.00s
#
# avrdude: Device signature = 0x1e9587 (probably m32u4)
# avrdude: reading input file "/var/folders/sd/khh05h_j2d3f99f29dpwbk8w0000gn/T/arduino_build_664548/har_man_devboard.ino.hex"
# avrdude: writing flash (8086 bytes):
#
# Writing | ################################################## | 100% 0.63s
#
# avrdude: 8086 bytes of flash written
# avrdude: verifying flash memory against /var/folders/sd/khh05h_j2d3f99f29dpwbk8w0000gn/T/arduino_build_664548/har_man_devboard.ino.hex:
# avrdude: load data flash data from input file /var/folders/sd/khh05h_j2d3f99f29dpwbk8w0000gn/T/arduino_build_664548/har_man_devboard.ino.hex:
# avrdude: input file /var/folders/sd/khh05h_j2d3f99f29dpwbk8w0000gn/T/arduino_build_664548/har_man_devboard.ino.hex contains 8086 bytes
# avrdude: reading on-chip flash data:
#
# Reading | ################################################## | 100% 0.08s
#
# avrdude: verifying ...
# avrdude: 8086 bytes of flash verified
#
# avrdude done.  Thank you.
#
#
