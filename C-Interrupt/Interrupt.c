#include "Interrupt.h"  

void handle_gpio_interrupt() {
    int status = system("libcamera-still -o capture.jpg");
    if (status == -1) {
        perror("system err");
        printf("Capture Failed\n");
    }
    printf("Capture success!\n");
}

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    struct gpiod_line_event event;
    int ret;

    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        perror("Failed to open GPIO chip");
        return EXIT_FAILURE;
    }

    line = gpiod_chip_get_line(chip, GPIO_PIN);
    if (!line) {
        perror("Failed to get GPIO line");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }
    ret = gpiod_line_request_events(line, GPIOD_LINE_REQUEST_EVENT_FALLING);
    if (ret < 0) {
        perror("Failed to request events");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }

    printf("Waiting for GPIO interrupt on pin %d...\n", GPIO_PIN);
    while (1) {
        ret = gpiod_line_event_wait(line, NULL);
        if (ret < 0) {
            perror("Error waiting for event");
            break;
        } else if (ret > 0) {
            gpiod_line_event_read(line, &event);
            handle_gpio_interrupt();
        }
    }
    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return EXIT_SUCCESS;
}
