CC       = gcc
CFLAGS   = -std=c11 -Wall -Isrc
LDFLAGS  = -lncurses

SRC      = src/main.c src/input.c src/ai.c src/render.c \
           src/physics.c src/score.c src/cleanup.c
OBJ      = $(SRC:.c=.o)

TEST_SRC = tests/test_physics.c tests/test_ai.c
UNITY    = tests/unity/unity.c
TEST_OBJ = $(TEST_SRC:.c=.o) $(UNITY:.c=.o)

.PHONY: compile test clean

compile: pong

pong: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test: test_runner
	./test_runner

test_runner: $(TEST_OBJ) src/physics.o src/ai.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f pong test_runner $(OBJ) $(TEST_OBJ)