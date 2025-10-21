// producer.c
// Producer process for bounded buffer (table size 2) using shared memory,
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
#include <string.h>
#include <time.h>
#include <errno.h>

#define SHM_NAME "/pc_table_shm"
#define TABLE_SIZE 2

typedef struct {
    int table[TABLE_SIZE];          // the “table” with 2 slots
    int count;                      // current number of items on table
    sem_t empty;                    // counting semaphore: empty slots
    sem_t full;                     // counting semaphore: full slots
    pthread_mutex_t mutex;          // process-shared mutex for critical section
} shared_data;

int main(void) {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) { perror("shm_open"); exit(1); }

    if (ftruncate(shm_fd, sizeof(shared_data)) == -1) {
        perror("ftruncate"); exit(1);
    }

    shared_data *shared = mmap(NULL, sizeof(shared_data),
                               PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared == MAP_FAILED) { perror("mmap"); exit(1); }

    // To avoid double-initialization if you rerun producer, zero first then init.
    // (If you need stronger single-initializer semantics, add a named semaphore gate.)
    memset(shared, 0, sizeof(*shared));

    // Init process-shared mutex
    pthread_mutexattr_t mattr;
    if (pthread_mutexattr_init(&mattr) != 0) { perror("pthread_mutexattr_init"); exit(1); }
    if (pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED) != 0) {
        perror("pthread_mutexattr_setpshared"); exit(1);
    }
    if (pthread_mutex_init(&shared->mutex, &mattr) != 0) {
        perror("pthread_mutex_init"); exit(1);
    }
    pthread_mutexattr_destroy(&mattr);

    // Init semaphores as process-shared
    if (sem_init(&shared->empty, 1, TABLE_SIZE) == -1) { perror("sem_init empty"); exit(1); }
    if (sem_init(&shared->full,  1, 0)          == -1) { perror("sem_init full");  exit(1); }

    shared->count = 0;

    srand((unsigned)time(NULL));
    puts("Producer: started");

    // Produce forever
    for (;;) {
        int item = rand() % 1000;

        // Wait for an empty slot, then lock the critical section
        if (sem_wait(&shared->empty) == -1) { perror("sem_wait empty"); exit(1); }
        if (pthread_mutex_lock(&shared->mutex) != 0) { perror("pthread_mutex_lock"); exit(1); }

        // Place item on the table (stack-like placement is fine for size 2)
        shared->table[shared->count] = item;
        shared->count++;
        printf("Producer: placed %d (count=%d)\n", item, shared->count);
        fflush(stdout);

        if (pthread_mutex_unlock(&shared->mutex) != 0) { perror("pthread_mutex_unlock"); exit(1); }
        if (sem_post(&shared->full) == -1) { perror("sem_post full"); exit(1); }

        sleep(1); // slow down so you can watch
    }

    // Not reached in normal run, but here for completeness
    munmap(shared, sizeof(*shared));
    shm_unlink(SHM_NAME);
    return 0;
}

