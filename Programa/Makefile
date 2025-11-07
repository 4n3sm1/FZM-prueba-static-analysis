CC ?= gcc
CFLAGS ?= -Wall -Wextra -Wconversion -Wshadow -O2 -std=c11
LDFLAGS ?= -lpthread
BUILD = build
BIN = bin
SRC = src
TESTS = tests

SRCS = $(SRC)/main.c $(SRC)/buffer.c $(SRC)/crc.c $(SRC)/config.c
OBJS = $(BUILD)/main.o $(BUILD)/buffer.o $(BUILD)/crc.o $(BUILD)/config.o

TEST_SRCS = $(TESTS)/test.c
TEST_OBJS = $(BUILD)/test.o

all: dirs $(BIN)/app

dirs:
\tmkdir -p $(BUILD) $(BIN)

$(BIN)/app: $(OBJS)
\t$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD)/%.o: $(SRC)/%.c
\t$(CC) $(CFLAGS) -I$(SRC) -c $< -o $@

test: dirs $(BIN)/tests
\t$(BIN)/tests

$(BIN)/tests: $(TEST_OBJS) $(BUILD)/buffer.o $(BUILD)/crc.o $(BUILD)/config.o
\t$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD)/test.o: $(TESTS)/test.c
\t$(CC) $(CFLAGS) -I$(SRC) -c $< -o $@

clean:
\trm -rf $(BUILD) $(BIN)
