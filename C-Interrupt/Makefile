CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = -lpigpio -pthread
TARGET = gpio_interrupt_program
SRCS = PollGPIO.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)
%.o: %.c PollGPIO.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJS) $(TARGET)
.PHONY: all clean run
