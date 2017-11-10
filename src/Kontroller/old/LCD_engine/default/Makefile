###############################################################################
# Makefile for the project LCD_engine
###############################################################################

## General Flags
PROJECT = LCD_engine
MCU = atmega1284p
TARGET = LCD_engine.elf
CC = avr-gcc

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99           -DF_CPU=3686400UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=LCD_engine.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = main_entry.o lcd_engine.o buttons.o games.o touch.o gui.o game_missile.o demo3d.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) LCD_engine.hex LCD_engine.eep LCD_engine.lss size

## Compile
main_entry.o: ../main_entry.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_engine.o: ../lcd_engine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

buttons.o: ../buttons.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

games.o: ../games.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

touch.o: ../touch.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

gui.o: ../gui.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

game_missile.o: ../game_missile.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

demo3d.o: ../demo3d.c
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
	-rm -rf $(OBJECTS) LCD_engine.elf dep/* LCD_engine.hex LCD_engine.eep LCD_engine.lss LCD_engine.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

