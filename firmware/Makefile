
BUILD_PATH = $(abspath ./.build)
BUILD_CACHE = $(abspath ./.cache)

FQBN = arduino:avr:leonardo

MCU = atmega32u4
# IDE = 10805
IDE = 10810

PORT ?= /dev/cu.usbmodem1421

PROJECT_ROOT = $(CURDIR)
PROJECT_NAME = $(notdir $(PROJECT_ROOT))

ARDUINO_APP = /Applications/Arduino.app
ARDUINO_ROOT = $(ARDUINO_APP)/Contents/Java

ARDUINO_BUILDER = $(ARDUINO_ROOT)/arduino-builder
# ARDUINO_BUILDER = $(HOME)/projects/arduino-builder/arduino-builder

TOOLS_BUILDER = $(ARDUINO_ROOT)/tools-builder
TOOLS_AVR = $(ARDUINO_ROOT)/hardware/tools/avr

LOCAL_PACKAGES = $(HOME)/Library/Arduino15/packages

AVR_GCC_PATH = $(ARDUINO_ROOT)/hardware/tools/avr

ifneq ("$(wildcard $(LOCAL_PACKAGES)/arduino/tools/avrdude/6.3.0-arduino17)", "")
# AVRDUDE_PATH = $(LOCAL_PACKAGES)/arduino/tools/avrdude/6.3.0-arduino14
AVRDUDE_PATH = $(LOCAL_PACKAGES)/arduino/tools/avrdude/6.3.0-arduino17
else
AVRDUDE_PATH = $(ARDUINO_ROOT)/hardware/tools/avr
endif

ifneq ("$(wildcard $(LOCAL_PACKAGES)/arduino/tools/arduinoOTA/1.3.0)", "")
# ARDUINO_OTA_PATH = $(LOCAL_PACKAGES)/arduino/tools/arduinoOTA/1.2.1
ARDUINO_OTA_PATH = $(LOCAL_PACKAGES)/arduino/tools/arduinoOTA/1.3.0
else
ARDUINO_OTA_PATH = $(ARDUINO_ROOT)/hardware/tools/avr
endif

CORE_INCLUDE = $(ARDUINO_ROOT)/hardware/arduino/avr/cores/arduino
LEONARDO_INCLUDE = $(ARDUINO_ROOT)/hardware/arduino/avr/variants/leonardo
EEPROM_INCLUDE = $(ARDUINO_ROOT)/hardware/arduino/avr/libraries/EEPROM/src
LCD_INCLUDE = $(ARDUINO_ROOT)/libraries/LiquidCrystal/src

ifneq ("$(wildcard $(LOCAL_PACKAGES))", "")
ARDUINO_HARDWARE = \
    $(ARDUINO_ROOT)/hardware \
    $(LOCAL_PACKAGES)

ARDUINO_TOOLS = \
    $(ARDUINO_ROOT)/tools-builder \
    $(ARDUINO_ROOT)/hardware/tools/avr \
    $(LOCAL_PACKAGES)
else
ARDUINO_HARDWARE = $(ARDUINO_ROOT)/hardware

ARDUINO_TOOLS = \
    $(ARDUINO_ROOT)/tools-builder \
    $(ARDUINO_ROOT)/hardware/tools/avr
endif

ARDUINO_PREFS = \
    build.warn_data_percentage=75 \
    runtime.tools.arduinoOTA.path=$(ARDUINO_OTA_PATH) \
    runtime.tools.arduinoOTA-1.3.0.path=$(ARDUINO_OTA_PATH) \
    runtime.tools.avr-gcc.path=$(AVR_GCC_PATH) \
    runtime.tools.avr-gcc-7.3.0-atmel3.6.1-arduino5.path=$(AVR_GCC_PATH) \
    runtime.tools.avrdude.path=$(AVRDUDE_PATH) \
    runtime.tools.avrdude-6.3.0-arduino17.path=$(AVRDUDE_PATH) \

ARDUINO_BUILDER_ARGS = \
    -logger=human \
    $(addprefix -hardware , $(ARDUINO_HARDWARE)) \
    $(addprefix -tools , $(ARDUINO_TOOLS)) \
    -built-in-libraries $(ARDUINO_ROOT)/libraries \
    -libraries $(HOME)/Documents/Arduino/libraries \
    -fqbn=$(FQBN) \
    -vid-pid=0X2341_0X8036 \
    -ide-version=$(IDE) \
    -build-path $(BUILD_PATH) \
    -warnings=all \
    -build-cache $(BUILD_CACHE) \
    $(addprefix --prefs=, $(ARDUINO_PREFS)) \
    -verbose $(PROJECT_ROOT)/$(PROJECT_NAME).ino \

AVR_GXX="$(AVR_GCC_PATH)/bin/avr-g++"

CPPFLAGS = -std=c++11 -xc++ -MMD -lncurses -Wall

.PHONY: all compile prefs upload dirs clean

all: firmware
# compile

CXXFLAGS = -g

firmware: mock/main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LOADLIBES) $(LDLIBS) -o $@ $<

-include firmware.d

$(BUILD_PATH)/$(PROJECT_NAME).ino.hex: compile

dirs: $(BUILD_PATH) $(BUILD_CACHE)

$(BUILD_PATH) $(BUILD_CACHE):
	mkdir -p $@

clean:
	rm -rf $(BUILD_PATH) $(BUILD_CACHE) firmware firmware.d

prefs: dirs
	$(ARDUINO_BUILDER) -dump-prefs $(ARDUINO_BUILDER_ARGS)

compile: dirs
	$(ARDUINO_BUILDER) -compile $(ARDUINO_BUILDER_ARGS)

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
    "-I$(CORE_INCLUDE)" \
    "-I$(LEONARDO_INCLUDE)" \
    "-I$(EEPROM_INCLUDE)" \
    "-I$(LCD_INCLUDE)" \
    "$(PROJECT_NAME).ino" \
    -o "$(BUILD_PATH)/sketch/$(PROJECT_NAME).ino.o" \

-include "$(BUILD_PATH)/sketch/$(PROJECT_NAME).ino.d"

upload: $(BUILD_PATH)/$(PROJECT_NAME).ino.hex
	$(AVRDUDE_PATH)/bin/avrdude \
    -C$(AVRDUDE_PATH)/etc/avrdude.conf \
    -v \
    -p$(MCU) \
    -cavr109 \
    -P$(PORT) \
    -b57600 \
    -D \
    -Uflash:w:$(BUILD_PATH)/$(PROJECT_NAME).ino.hex:i \

