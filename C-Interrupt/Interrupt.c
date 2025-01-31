#include "Interrupt.h"
void gpio_interrupt() {
    int status = system("libcamera-still -o capture.jpg");
    if (status == -1) {
        perror("system");
        return 1;
    }
    printf("capture occurred\n");
    return 0;

}

void setup_gpio(){ // Setup gpio listener
    wiringPiSetup();
    wiringPiSetupGpio();
    pinMode(GPIO_PIN, INPUT);
    pullUpDnControl(GPIO_PIN, PUD_UP);

}

int main() {
    setup_gpio();
    writingPiISR(GPIO_PIN, INT_EDGE_FALLING, &gpio_interrupt);
    while(1){
        sleep(1);
    }
    return 0;
}