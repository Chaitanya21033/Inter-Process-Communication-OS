#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define FIFO_NAME "one"
#define FIFO_NAME2 "two"

char strings[6 * 50];
int num, fd, fd2;
clock_t begin;

double time_spent = 0.0;

void receive_index()
{
    fd2 = open(FIFO_NAME2, O_RDONLY);
    int c[1];
    do
    {
        if ((num = read(fd2, c, sizeof(c))) == -1)
            perror("read");
        else
        {
        }
    } while (num > 0);
    close(fd2);
    int garbae = func(c[0] + 1);
    if (!garbae)
    {
        return;
    }
}

int func(int res)
{
    if (res > 279)
    {
        printf("YOU are done here");
        clock_t end = clock();

        // timepsec here and
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

        printf("\nThe elapsed time is %f seconds", time_spent);

        return 0;
    }
    fd = open(FIFO_NAME, O_WRONLY);
    char s[6 * 5];
    int count = 0;
    int index = res;
    for (int i = res; i < res + 30; i++)
    {
        s[count] = strings[i];
        count++;
    }
    for (int i = 0; i < 30; i++)
    {
        printf("%c", s[i]);
    }
    printf("\n");

    if ((num = write(fd, s, sizeof(s))) == -1)
        perror("write");
    else
        printf("speak: wrote %d bytes\n", num);
    close(fd);
    receive_index();
}

int main(void)
{
    srand(time(NULL));
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);
    mknod(FIFO_NAME2, S_IFIFO | 0666, 0);
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
    begin = clock();

    int res = 0;
    func(res);

    return 0;
}