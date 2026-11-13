BUILD_DIR ?= build
SOURCE_DIR ?= src
INCLUDE_DIR ?= include

CC = gcc
CFLAGS ?= -std=c99 -Wall -Wextra -I$(INCLUDE_DIR)

TARGETS = student tui

OBJS = $(TARGETS:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(INCLUDE_DIR)/%.h | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

fifo_list: $(SOURCE_DIR)/fifo_list.c $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS)
