SRC = src/main.c \
      src/chacha/chacha.c \
			src/commands/commands.c \
			src/commands/decrypt/decrypt.c \
			src/commands/encrypt/encrypt.c \
			src/commands/random/random.c \
			src/input/input.c \
			src/opts/opts.c
			
OBJ = $(SRC:.c=.o)
BIN = bin/afet

CFLAGS += -Wall -Wextra
CFLAGS += -I $(CURDIR)/src

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(BIN) $(OBJ)