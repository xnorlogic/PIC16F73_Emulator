IDIR =include
ODIR =obj
SDIR =source

CC = gcc
CFLAGS = -I$(IDIR)

_DEPS = ALU.h PIC16F73.h
_OBJ = ALU.o PIC16F73.o EmulatorShell.o
_SOURCE = ALU.c PIC16F73.c EmulatorShell.c

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
SOURCE = $(patsubst %,$(SDIR)/%,$(_SOURCE))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ShellEXE: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
