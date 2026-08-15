CC = clang
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c2x -Iinclude -g -DDEBUG
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/jvm

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

# Java
JAVAC = javac

$(OBJ_DIR)/%.class: $(SRC_DIR)/%.java
	$(JAVAC) $< -d $(OBJ_DIR)

test/%.class: test/%.java
	$(JAVAC) $<

test: all test/Add.class
	@echo "Wacha"
	./$(TARGET) test/Add.class