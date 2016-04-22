##############################################################################
# Makefile for the project avr-adctest
###############################################################################

## NOTE: GNU Make only

## General Flags
PROJECT = avr-lcdtest
MCU = atmega328p
TARGET = avr-lcdtest.elf
CC = avr-gcc
CXX = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU) 

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -DF_CPU=16000000UL -D__AVR_ATmega328P__
CFLAGS += -Wall -Os -g -DF_CPU=16000000UL -D__AVR_ATmega328P__
CFLAGS += -MD -MP -MT $(*F).o -MF $(@F).d 

CXXFLAGS = $(COMMON)
CXXFLAGS += -DF_CPU=16000000UL -D__AVR_ATmega328P__
CXXFLAGS += -MD -MP -MT $(*F).o -MF $(@F).d 

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += -Wl,-Map=avr-lcdtest.map

## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

## Objects that must be built in order to link
OBJECTS = avr-lcdtest.o \
	AVRTools/I2cLcd.o \
	AVRTools/I2cMaster.o \
	AVRTools/InitSystem.o \
	AVRTools/Reader.o \
	AVRTools/RingBuffer.o \
	AVRTools/SystemClock.o \
	AVRTools/USART0.o \
	AVRTools/USART0Minimal.o \
	AVRTools/Writer.o \
	AVRTools/abi.o \
	AVRTools/new.o

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) avr-lcdtest.hex avr-lcdtest.eep avr-lcdtest.lss size

## Compile

.c.o:
	$(CC) -c $< $(CFLAGS) $(LDFLAGS) -o $@

.cpp.o:
	$(CXX) -c $< $(CXXFLAGS) $(LDFLAGS) -o $@

.S.o:
	$(CC) -c $< $(CFLAGS) $(LDFLAGS) -o $@

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size ${TARGET}
#	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) avr-lcdtest.elf dep/* avr-lcdtest.hex avr-lcdtest.eep avr-lcdtest.lss avr-lcdtest.map
