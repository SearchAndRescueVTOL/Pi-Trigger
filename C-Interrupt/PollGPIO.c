#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
// 1. Spawn a thread listening for pps event, 2 threads waiting on data to a shared memory buffer, 1 consumer thread to that buffer that sends data over to jetson
// 2. On pps, this thread adds pps event to rgb and ir buffers, where threads are waiting, they trigger the cameras and record tick time when images are received/before sending trigger signal, then add results to buffer(s) for consumer thread to send to jetson
#include <pigpio.h>
#include <stdio.h>
#include <stdatomic.h>

#define DEFAULT_SAMPLE_RATE 1
#define BUFFER_SIZE 10000 // idk wtf
atomic_int ready = 2;
uint32_t 
typedef struct {
    uint32_t tickBuffer[BUFFER_SIZE];
    atomic_int head;
    atomic_int tail;
} ProdConsBuffer;
ProdConsBuffer rgb = { .head = 0, .tail = 0 };
ProdConsBuffer ir = { .head = 0, .tail = 0 };

void aFunction(int gpio, int level, uint32_t tick) {
    /* only record low to high edges */
    if (level == 1) {
        // RGB time
        int nextRGB = (atomic_load_explicit(&rgb.head, memory_order_relaxed) + 1) % BUFFER_SIZE;
        int nextIR = (atomic_load_explicit(&ir.head, memory_order_relaxed) + 1) % BUFFER_SIZE;
        while((nextRGB == atomic_load_explicit(&rgb.tail, memory_order_acquire)) || (nextIR == atomic_load_explicit(&ir.tail, memory_order_acquire))) { // maybe replace this logic with a barrier? 
            pthread_yield();
        }
        rgb.tickBuffer[rgb.head] = tick;
        ir.tickBuffer[ir.head] = tick;
        atomic_store_explicit(&rgb.head, nextRGB, memory_order_release);
        atomic_store_explicit(&ir.head, nextIR, memory_order_release);
        // Add pps signal to rgbbuffer with tick time
        // add pps signal to IR buffer with tick time  
    }
    return NULL;
}
void *rgb_trigger(void *arg){
    return;
}

void *ir_trigger(void *arg){
    return;
}
void *sendToJetson(void *arg){
    return;
}
int main() {
    pthread_t rgb;
    pthread_t ir;
    pthread_t consume;
    if (pthread_create(&rgb, NULL, rgb_trigger, NULL) != 0) {
        printf("Failed to create thread\n");
        return 1;
    }
    if (pthread_create(&ir, NULL, ir_trigger, NULL) != 0){
        printf("Failed to create thread\n");
        return 1;
    }
    if (pthread_create(&consume, NULL, sendToJetson, NULL) != 0){
        printf("Failed to create thread\n");
        return 1;
    }

    gpioCfgClock(DEFAULT_SAMPLE_RATE, 1, 1);
    if (gpioInitialise() < 0) {
        return 1;
    }
    int mode;
    
    gpioWaveClear();
    gpioSetMode(4, PI_INPUT); // for gpio pin 4 (broadcom numbered)
    gpioSetAlertFunc(4, aFunction);    // for GPIO pin 4
    while(1){
        pthread_yield();
    }
    gpioTerminate();
    pthread_join(rgb, NULL);
    pthread_join(ir, NULL);
    pthread_join(consume, NULL);
    return 0;
}