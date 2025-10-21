// consumer.c
// Consumer process for bounded buffer (table size 2) using shared memory,
// counting semaphores (empty/full), and a process-shared pthread mutex.

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>

#define SHM_NAME "/pc_table_shm"
#define TABLE_SIZE 2

typedef struct {
    int table[TABLE_SIZE];
    int count;
    sem_t empty;
    sem_t full;
    pthread_mutex_t mutex;
} shared_data;

int main(void) {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open (start producer first)"); exit(1);
    }

    shared_data *shared = mmap(NULL, sizeof(shared_data),
                               PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared == MAP_FAILED) { perror("mmap"); exit(1); }

    puts("Consumer: started");

    for (;;) {
        // Wait for an available item, then lock
        if (sem_wait(&shared->full) == -1) { perror("sem_wait full"); exit(1); }
        if (pthread_mutex_lock(&shared->mutex) != 0) { perror("pthread_mutex_lock"); exit(1); }

        // Take item from table
        shared->count--;
        int item = shared->table[shared->count];
        printf("Consumer: took  %d (count=%d)\n", item, shared->count);
        fflush(stdout);

        if (pthread_mutex_unlock(&shared->mutex) != 0) { perror("pthread_mutex_unlock"); exit(1); }
        if (sem_post(&shared->empty) == -1) { perror("sem_post empty"); exit(1); }

        sleep(2); // slower than producer to show blocking behavior
    }

    // Not reached in normal run
    munmap(shared, sizeof(*shared));
    return 0;
}

