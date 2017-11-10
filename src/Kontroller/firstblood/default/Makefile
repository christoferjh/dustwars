###############################################################################
# Makefile for the project zigdust
###############################################################################

## General Flags
PROJECT = zigdust
MCU = atmega1284p
TARGET = zigdust.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99     -DF_CPU=1000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=zigdust.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = zigdust.o buttons.o gui.o lcd_engine.o touch.o zigbeeny.o accelerometer.o game_missile.o ball_game.o killer.o images.o settings.o debug.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) zigdust.hex zigdust.eep zigdust.lss size

## Compile
zigdust.o: ../zigdust.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

buttons.o: ../buttons.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

gui.o: ../gui.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_engine.o: ../lcd_engine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

touch.o: ../touch.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

zigbeeny.o: ../zigbeeny.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

accelerometer.o: ../accelerometer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

game_missile.o: ../game_missile.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ball_game.o: ../ball_game.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

killer.o: ../killer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

images.o: ../images.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

settings.o: ../settings.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

debug.o: ../debug.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) zigdust.elf dep/* zigdust.hex zigdust.eep zigdust.lss zigdust.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

