# --------------------------------------------------
# Console Pong – Ordner-aware Makefile
# --------------------------------------------------

CC      := gcc
CFLAGS  := -Wall -Wextra -pedantic -std=c11
LDFLAGS := -lncurses

SRC_DIR := src
OBJ_DIR := build

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
BIN := pong

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Erzeuge build-Ordner bei Bedarf und baue jedes .c zu einem .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean