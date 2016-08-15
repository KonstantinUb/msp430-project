UNITS		= main tui serial convert timers
SOURCES		= $(addsuffix .c,$(UNITS))
OBJECTS		= $(addsuffix .o,$(UNITS))
ASSEMBLIES	= $(addsuffix .s,$(UNITS))

GCC_DIR		= C:/Users/Konstantin/ti/gcc
BIN_DIR		= $(GCC_DIR)/bin
INCLUDE_DIR	= $(GCC_DIR)/include

DEVICE	= msp430f5438a
CC		= $(BIN_DIR)/msp430-elf-gcc
GDB		= $(BIN_DIR)/msp430-elf-gdb

CFLAGS	= -I $(INCLUDE_DIR) -mmcu=$(DEVICE) -O2 -g
LFLAGS	= -L $(INCLUDE_DIR)

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $? -o $(DEVICE).out

debug: all
	$(GDB) $(DEVICE).out

assembly: $(SOURCES)
	$(CC) $(CFLAGS) $(LFLAGS) $? -S

merge: $(SOURCES)
	type *.h $(SOURCES) > _merge.c 2>nul
	$(CC) $(CFLAGS) -c _merge.c
	$(CC) $(CFLAGS) _merge.c -S

.PHONY: clean
clean:
	del $(OBJECTS)
	del $(ASSEMBLIES)
	del _merge.c _merge.o _merge.s
