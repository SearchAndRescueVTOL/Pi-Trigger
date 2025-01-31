#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GPIO_CHIP "/dev/gpiochip0"  
#define GPIO_PIN  17

// Function to handle the GPIO interrupt
void handle_gpio_interrupt();

// Function to initialize GPIO and request events
int setup_gpio_interrupt();

#endif 
