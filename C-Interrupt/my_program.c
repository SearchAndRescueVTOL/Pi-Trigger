#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>

#define NUM_THREADS 4

void *thread_func(void *arg) {
    int cpu_id = *(int *)arg;
    cpu_set_t cpuset;
    
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_id, &cpuset);

    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np failed");
    }

    // Check which CPU the thread is running on
    printf("Thread %d running on CPU %d\n", cpu_id, sched_getcpu());

    while (1) { 
        // Simulate work
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int cpu_ids[NUM_THREADS] = {0, 1, 2, 3};  // Assigning each thread to one CPU

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, &cpu_ids[i]) != 0) {
            perror("pthread_create failed");
            return 1;
        }
        usleep(1000);  // Small delay to ensure threads are scheduled properly
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
