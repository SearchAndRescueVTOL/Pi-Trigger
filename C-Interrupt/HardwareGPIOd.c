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

    // Configure GPIO interrupt request
    struct gpiod_line_request_config config;
    config.consumer = "gpio_interrupt";
    config.request_type = GPIOD_LINE_REQUEST_EVENT_BOTH_EDGES; // Corrected constant
    config.flags = 0;  // No additional flags

    int ret = gpiod_line_request(line, &config, 0);
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
    struct timespec ts;
    int ret;

    
    FILE *fd = fopen("output.txt", "w");
    if (fd == NULL){
	    perror("Failed to open file");
    }
//fprintf(fd, "Listening for GPIO %d interrupts (non-blocking)...\n", GPIO_LINE);
    fflush(fd);
    
    while (1) {
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_nsec = 0;

        timespec_get(&ts,TIME_UTC);

       // struct tm *delta = gmtime(&ts.tv_nsec);

        ret = gpiod_line_event_wait(line, &timeout);
        if (ret < 0) {
            perror("Error waiting for GPIO event");
            break;
        } else if (ret == 0) {
            //fprintf(fd, "No GPIO event detected within %d seconds. Continuing...\n", TIMEOUT_SEC);
	    fflush(fd);
            continue;
        }

        ret = gpiod_line_event_read(line, &event);
        if (ret < 0) {
            perror("Error reading GPIO event");
            break;
        }

        if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE) {
            //printf("GPIO %d triggered! Type: RISING EDGE at time %ld \n", GPIO_LINE, ts.tv_nsec);
            fprintf(fd, "%lld \n",((long long)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL));
	    fflush(fd);
        } //else if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
        //     printf("GPIO %d triggered! Type: FALLING EDGE at time %ld \n", GPIO_LINE, ts.tv_nsec);
        // }
    }
}

// Function to clean up GPIO resources
void cleanup_gpio(struct gpiod_chip *chip, struct gpiod_line *line) {
    gpiod_line_release(line);
    gpiod_chip_close(chip);
    printf("GPIO cleanup completed.\n");
}


int main(void) {
    struct gpiod_chip *chip = open_gpio_chip();
    struct gpiod_line *line = configure_gpio_interrupt(chip, GPIO_LINE);

    while(1){
        handle_gpio_interrupt(line);
        cleanup_gpio(chip, line);
        //HAL_Delay(500);
    }
    return 0;
}
