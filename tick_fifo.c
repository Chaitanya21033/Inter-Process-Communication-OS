#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
int num, fd, fd2;

#define FIFO_NAME "one"
#define FIFO_NAME2 "two"

void send_index(int index)
{
    int c[1];
    c[0] = index;

    fd2 = open(FIFO_NAME2, O_WRONLY);
    if ((num = write(fd2, c, sizeof(c))) == -1)
        perror("write");
    else

        close(fd2);
    func();
}

void func()
{
    char s[6 * 5];
    fd = open(FIFO_NAME, O_RDONLY);

    int index = 0;
    char reading[30];
    do
    {

        if ((num = read(fd, s, 30)) == -1)
            perror("read");
        else
        {

            for (int i = 0; i < num; i++)
            {
                reading[index] = s[i];
                index++;
            }
        }
    } while (num > 0);
    int max_index = 0;
    index = 0;
    for (int i = 0; i < 30; i++)
    {
        if (i % 6 == 0)
        {
            if (max_index < reading[i])
            {
                max_index = reading[i];
                index = i;
            }
        }
    }
    printf("MAX INDEX: %c\n", reading[index]);

    printf("STRING: ");
    for (int i = index + 1; i < index + 1 + 5; i++)
    {
        printf("%c", reading[i]);
    }
    printf("\n");
    close(fd);
    index = reading[index] - 'A' + 1;
    index *= 6;
    index -= 1;
    send_index(index);
}
int main(void)
{
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);
    mknod(FIFO_NAME2, S_IFIFO | 0666, 0);
    func();
    return 0;
}