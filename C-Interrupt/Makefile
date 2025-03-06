CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LIBS = -lpigpio -pthread
TARGET = gpio_interrupt_program
SRCS = PollGPIO.c
OBJS = $(SRCS:.c=.o) 
PKGCONFIG = $(shell pkg-config --cflags --libs gstreamer-1.0)
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS) $(PKGCONFIG)
%.o: %.c PollGPIO.h
	$(CC) $(CFLAGS) -c $< -o $@ $(PKGCONFIG)
clean:
	rm -f $(OBJS) $(TARGET)
.PHONY: all clean run
