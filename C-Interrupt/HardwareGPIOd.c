#include "HardwareGPIOd.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Function to open the GPIO chip
struct gpiod_chip* open_gpio_chip() {
    struct gpiod_chip *chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        perror("Failed to open GPIO chip");
        exit(EXIT_FAILURE);
    }
    return chip;
}

// Function to configure GPIO line for interrupts
struct gpiod_line* configure_gpio_interrupt(struct gpiod_chip *chip, int line_number) {
    struct gpiod_line *line = gpiod_chip_get_line(chip, line_number);
    if (!line) {
        perror("Failed to get GPIO line");
        gpiod_chip_close(chip);
        exit(EXIT_FAILURE);
    }

    int ret = gpiod_line_request_events(line, GPIOD_LINE_REQUEST_BOTH_EDGES, "gpio_interrupt");
    if (ret < 0) {
        perror("Failed to request GPIO line events");
        gpiod_chip_close(chip);
        exit(EXIT_FAILURE);
    }

    return line;
}

// Function to handle GPIO interrupts in a non-blocking way
void handle_gpio_interrupt(struct gpiod_line *line) {
    struct gpiod_line_event event;
    struct timespec timeout;
    int ret;

    printf("Listening for GPIO %d interrupts (non-blocking)...\n", GPIO_LINE);

    while (1) {
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_nsec = 0;

        ret = gpiod_line_event_wait(line, &timeout);
        if (ret < 0) {
            perror("Error waiting for GPIO event");
            break;
        } else if (ret == 0) {
            printf("No GPIO event detected within %d seconds. Continuing...\n", TIMEOUT_SEC);
            continue;
        }

        ret = gpiod_line_event_read(line, &event);
        if (ret < 0) {
            perror("Error reading GPIO event");
            break;
        }

        if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE) {
            printf("GPIO %d triggered! Type: RISING EDGE\n", GPIO_LINE);
        } else if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
            printf("GPIO %d triggered! Type: FALLING EDGE\n", GPIO_LINE);
        }
    }
}

// Function to clean up GPIO resources
void cleanup_gpio(struct gpiod_chip *chip, struct gpiod_line *line) {
    gpiod_line_release(line);
    gpiod_chip_close(chip);
    printf("GPIO cleanup completed.\n");
}


int main() {
    struct gpiod_chip *chip = open_gpio_chip();
    struct gpiod_line *line = configure_gpio_interrupt(chip, GPIO_LINE);
    
    handle_gpio_interrupt(line);

    cleanup_gpio(chip, line);
    return 0;
}