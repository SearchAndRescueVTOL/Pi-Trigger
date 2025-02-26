# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -std=c11 -I/usr/include

# Libraries
LIBS = -lgpiod -pthread

# Target Executable Name
TARGET = gpio_interrupt_program

# Source and Object Files
SRCS = HardwareGPIOd.c
OBJS = $(SRCS:.c=.o)

# Default Target
all: $(TARGET)

# Linking Step
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# Compilation Step
%.o: %.c HardwareGPIOd.h
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f $(OBJS) $(TARGET)

# Mark `all` and `clean` as non-file targets
.PHONY: all clean
