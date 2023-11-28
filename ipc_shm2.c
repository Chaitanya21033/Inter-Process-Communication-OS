#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

#define SHM_SIZE 4096
double time_spent = 0.0;
char strings[6 * 50];
int num, fd, fd2;
int main()
{
    key_t key = ftok("/tmp", 'A');

    int shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id < 0)
    {
        perror("shmget");
        return 1;
    }

    void *shm_ptr = shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *)-1)
    {
        perror("shmat");
        return 1;
    }

    sem_t *sem = (sem_t *)shm_ptr;
    if (sem_init(sem, 1, 1) < 0)
    {
        perror("sem_init");
        return 1;
    }

    sem_t *sem2 = (sem_t *)shm_ptr;
    if (sem_init(sem, 1, 1) < 0)
    {
        perror("sem_init");
        return 1;
    }
    srand(time(NULL));
	int index = 0;
	for (int i = 0; i < 300; i++)
	{
		if (i % 6 == 0)
		{
			index++;
			strings[i] = index + 'A' - 1;
			continue;
		}
		strings[i] = abs(rand() % 26) + 97;
	}
    for (int i = 0; i < 10; i++)
    {
        sem_wait(sem);
        sem_wait(sem2);
        char share[30];
		int count = 0;
		int index = 0;
		for (int i = index; i < index + 30; i++)
		{
			share[count] = strings[i];
			count++;
		}

        char *str = (char *)(shm_ptr + sizeof(sem_t));
        printf("Read from shared memory: %s\n", str);
        char str2[SHM_SIZE - sizeof(sem_t)];
        sprintf(str2, "Written to shared memory in else:  %d", i);
        memcpy(shm_ptr + sizeof(sem_t), str2, strlen(str2));
        sem_post(sem);
        sleep(0);
        sem_post(sem2);
        sleep(0);

    }

    return 0;
}
