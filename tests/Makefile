CC = gcc
CFLAGS = -std=c23 -I../include -Wall -Wextra -pedantic -O0 -g
LDFLAGS = -L../lib -lunit-check

SRC_DIRS = . ../src
OBJ_DIR = ./obj
BUILD_DIR = .

TARGET = $(BUILD_DIR)/tests

SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
OBJS = $(SRCS:%=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf ./src
	rm -f $(TARGET)

.PHONY: all clean
