#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#define SHM_SIZE 4096
#define NUM_ITERATIONS 10

int main()
{
    key_t key = ftok("/tmp", 'A');
    printf("HERE\n");
    int shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id < 0)
    {
        perror("shmget");
        return 1;
    }
    printf("HERE2\n");
    void *shm_ptr = shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *)-1)
    {
        perror("shmat");
        return 1;
    }
    printf("HERE3\n");
    sem_t *sem = malloc(sizeof(sem_t));
    sem = sem_open("/sem1", O_CREAT, 0666, 0);
    if (sem_init(sem, 1, 1) < 0)
    {
        perror("sem_init");
        return 1;
    }
    printf("HERE4\n");

    sem_t *sem2 = malloc(sizeof(sem_t));
    sem2 = sem_open("/sem2", O_CREAT, 0666, 0);

    if (sem_init(sem2, 1, 1) < 0)
    {
        perror("sem_init");
        return 1;
    }
    printf("HERE5\n");

    for (int i = 0; i < 1; i++)
    {
        sem_wait(sem);
        memset(shm_ptr, 0, 4096);
        printf("Start\n");
        sleep(5);
        char str[SHM_SIZE - sizeof(sem_t)];
        memset(shm_ptr, 0, 4096);
        sprintf(str, "raghva %d", i);
        memcpy(shm_ptr + sizeof(sem_t), str, strlen(str));
        sem_post(sem2);

        sem_post(sem);
        sem_wait(sem2);
        char *str2 = (char *)(shm_ptr + sizeof(sem_t));
        printf("Read from shared left side memory: %s\n", str2);
    }
    return 0;
}
