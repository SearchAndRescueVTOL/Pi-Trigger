CC = gcc
CFLAGS = -Wall -Wextra -std=c99

LIBS = -lwiringPi

TARGET = interrupt_program
SRCS = Interrupt.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)
%.o: %.c Interrupt.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean run