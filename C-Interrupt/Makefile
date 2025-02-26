CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = -pthread -lgpiod
TARGET = gpio_interrupt_program
SRCS = HardwareGPIOd.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)
%.o: %.c HardwareGPIOd.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJS) $(TARGET)
.PHONY: all clean run
