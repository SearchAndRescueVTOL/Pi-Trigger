#define _GNU_SOURCE // Required for CPU affinity functions
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <pigpio.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <gst/gst.h>
#define DEFAULT_SAMPLE_RATE 1
#define OUTPUT_FILE_NAME "output.txt"
pthread_barrier_t barrier;
uint32_t tickGlobal;
atomic_bool tickReady = ATOMIC_VAR_INIT(true);
int trigger_counter = 0;
uint32_t prev_tick = 0;
atomic_int recent_trigger_number = ATOMIC_VAR_INIT(0);
FILE *fd;
void aFunction(int gpio, int level, uint32_t tick) {
  /* only record low to high edges */
  if (level == 1) {
    trigger_counter += 1;
    fprintf(fd, "%llu \n", (unsigned long long) tick);
    fflush(fd);
	// RGB time
    bool exp = true;
    bool desired = false;
    if (atomic_compare_exchange_strong(&tickReady, &exp, desired)){
      tickGlobal = tick;
      atomic_store(&recent_trigger_number, trigger_counter);
      return;
    }
    else{
      return; // Drop frames when a trigger occurs while previous trigger still processing
    }
  }
  return;
}
void set_cpu_affinity(int core_id) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset); // Assign thread to core_id
  if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),&cpuset) != 0) {
    printf("Error getting affinity!\n");
  }
}
void *rgb_trigger(void *arg){
  int thread_id = *(int *)arg;
  set_cpu_affinity(thread_id);
  while(true){
    if (atomic_load(&tickReady) == false){
      uint32_t tick = tickGlobal;
      int trigger_num = atomic_load(&recent_trigger_number);
      pthread_barrier_wait(&barrier);
      // fucking actually trigger the camera here (im not writing this)
      uint32_t endTick = gpioTick();
      atomic_store(&tickReady, true);
      // send image capture, start time, end time, trigger number associated with capture all together as a packet to httpserver
    }
  }
  return;
}
void *ir_trigger(void *arg){
  int thread_id = *(int *)arg;
  set_cpu_affinity(thread_id);
  while(true){
    if (atomic_load(&tickReady) == false){
      uint32_t tick = tickGlobal;
      int trigger_num = atomic_load(&recent_trigger_number);
      pthread_barrier_wait(&barrier);
      // fucking actually trigger the ir camera here
      uint32_t endTick = gpioTick();
      atomic_store(&tickReady, true);
      // send image capture, start time, end time, trigger number associated with capture all together as a packet to httpserver
    }
  }
  return;
}
void *sendToJetson(void *arg){
  int thread_id = *(int *)arg;
  set_cpu_affinity(thread_id);
  return;
}
void *conductor(void *arg){
  pthread_t rgb;
  pthread_t ir;
  pthread_t consume;
  pthread_attr_t attr;
  struct sched_param param;
  set_cpu_affinity(3);
  pthread_attr_init(&attr);
  pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  param.sched_priority = sched_get_priority_max(SCHED_FIFO);
  pthread_attr_setschedparam(&attr, &param);
  pthread_barrier_init(&barrier, NULL, 2);
  int x = 0;
  if (pthread_create(&rgb, &attr, rgb_trigger, &x) != 0) {
    printf("Failed to create thread\n");
    return;
  }
  usleep(1000);
  int y = 1;
  if (pthread_create(&ir, &attr, ir_trigger, &y) != 0){
    printf("Failed to create thread\n");
    return;
  }
  usleep(1000);
  int z = 2;
  if (pthread_create(&consume, NULL, sendToJetson, &z) != 0){
    printf("Failed to create thread\n");
    return;
  }
  usleep(1000);
  gpioTerminate();
  gpioCfgClock(DEFAULT_SAMPLE_RATE, 1, 1);
  if (gpioInitialise() < 0) {
    return;
  }
  int mode;
  gpioWaveClear();
  gpioSetMode(27, PI_INPUT); // for gpio pin 4 (broadcom numbered)
  gpioSetAlertFunc(27, aFunction);  // for GPIO pin 4
  while(1){ // maybe not needed depending on how the call back works (if it creates a seperate thread for the callback on gpio or not)
    sleep(1);
  }
  gpioTerminate();
  pthread_join(rgb, NULL);
  pthread_join(ir, NULL);
  pthread_join(consume, NULL);
  pthread_attr_destroy(&attr);
  return 0;
}
int main() {
	// oracle code here
  fd = fopen(OUTPUT_FILE_NAME, "w");
  if (fd == NULL){
    perror("Failed to open file\n");
  }
  fflush(fd);
  pthread_t con;
  pthread_attr_t attr;
  struct sched_param param;
  pthread_attr_init(&attr);
  pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  param.sched_priority = sched_get_priority_max(SCHED_FIFO);
  pthread_attr_setschedparam(&attr, &param);
  if (pthread_create(&con, &attr, conductor, NULL) != 0){
    printf("Error\n");
    return 1;
  }
  usleep(1000);
  pthread_attr_destroy(&attr);
  pthread_join(con, NULL);

	return 0;
}

