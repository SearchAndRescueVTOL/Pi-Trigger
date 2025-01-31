#ifndef INTERRUPT_H
#define INTERRUPT_H
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#define GPIO_PIN 17

void gpio_interrupt(); // Interrupt Handler

void setup_gpio();


#endif