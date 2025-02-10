.PHONY: all clean

NAME = game.sfc

CFLAGS = -C lorom.cfg -t nes -Oisr --cpu 65816
ASFLAGS = --cpu 65816 -t nes
CC = cl65

SRC = $(wildcard *.c)
SRC += $(wildcard *.s)

OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:.s=.o)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.s
	$(CC) -c $(ASFLAGS) $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(CFLAGS) $(OBJ) snes.lib

$(OBJ): $(wildcard *.h *.sinc)

clean:
	rm -f $(NAME) *.o
