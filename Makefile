# Compiler settings
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -mconsole -I.
LDFLAGS = 

# Targets
TARGET = main2
TEST_TARGET = tests

# Source files
SRC = main2.c matrix.c type_int.c type_float.c type_complex.c commands1.c
TEST_SRC = tests.c matrix.c type_int.c type_float.c type_complex.c commands1.c

.PHONY: all clean

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	del /Q /F $(TARGET).exe $(TEST_TARGET).exe *.o 2> nul