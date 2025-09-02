CC = gcc
CFLAGS = -std=c23 -fPIC -I./include -Wall -Wextra -pedantic
LDFLAGS = -shared

OPTFLAGS ?= -O2 -DNDEBUG
CFLAGS += $(OPTFLAGS)

SRC_DIR = ./src
OBJ_DIR = ./obj
BUILD_DIR = ./lib
EXAMPLE_DIR = ./example
INSTALL_DIR ?= /usr/lib
HEADER_INSTALL_DIR ?= /usr/include

TARGET = $(BUILD_DIR)/libu8t.so
EXAMPLE = $(EXAMPLE_DIR)/example

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	@mkdir -p $(INSTALL_DIR)
	install -m 0755 $(TARGET) $(INSTALL_DIR)
	@mkdir -p $(HEADER_INSTALL_DIR)/u8t
	@cp -p include/ciphers/*.h $(HEADER_INSTALL_DIR)/u8t

uninstall:
	@rm -f $(INSTALL_DIR)/libu8t.so
	@rm -rf $(HEADER_INSTALL_DIR)/u8t

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

debug: OPTFLAGS = -O0 -g -DDEBUG -fno-omit-frame-pointer
debug: all

example: $(EXAMPLE)

$(EXAMPLE): $(EXAMPLE_DIR)/main.c $(TARGET)
	$(CC) -I./include -L$(BUILD_DIR) -lu8t -o $@ $<

compile_commands:
	bear -- make clean all

.PHONY: all clean install uninstall debug example compile_commands
