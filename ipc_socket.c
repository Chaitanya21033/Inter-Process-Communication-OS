#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>

#define SOCK_PATH "echo_socket"

int counter = 0;
clock_t begin;
clock_t end;

double time_spent = 0.0;
char strings[6 * 50];
int num, fd, fd2;

int main(void)
{
	FILE *fp;
	fp = fopen("ipc_logs.txt", "w");
	if (fp == NULL)
		printf("Error Occurred while Opening/Creating the File!");
	int sv[2];
	char buf;
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
	int index_to_begin = 0;
	begin = clock();

	for (int j = 0; j < 10; j++)
	{
		char share[30];
		int count = 0;
		int index = 0;
		for (int i = index_to_begin; i < index_to_begin + 30; i++)
		{
			share[count] = strings[i];
			count++;
		}
		fprintf(fp, "%s", "INPUT SENT TO P2: ");
		for (int i = 0; i < 30; i++)
		{
			fprintf(fp, "%c", share[i]);
		
		}
		fprintf(fp, "%c", '\n');
	

		if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1)
		{
			perror("socketpair");
			exit(1);
		}

		pid_t child_pid, wpid;
		int status = 0;

		if ((child_pid = fork()) == 0)
		{
			read(sv[1], &share, 1);
		
			int max_index = 0;
			int index2 = 0;
			for (int l = 0; l < 30; l++)
			{
				if (l % 6 == 0)
				{
					if (max_index < share[l])
					{
						max_index = share[l];
						index2 = l;
					}
				}
			}
			fprintf(fp, "%s", "MAX INDEX: ");
			fprintf(fp, "%c\n", share[index2]);
			fprintf(fp, "%s", "STRING: ");
			printf("MAX INDEX: %c\n", share[index2]);

			printf("STRING: ");
			for (int l = index2 + 1; l < index2 + 1 + 5; l++)
			{
				printf("%c", share[l]);
				fprintf(fp, "%c", share[l]);
			}
			fprintf(fp, "%c", '\n');
			printf("\n");
			write(sv[1], &share[index2], 1);
		
		}
		else
		{
			write(sv[0], share, 1);
		
			read(sv[0], &buf, 1);
		
			index_to_begin = (buf - 'A' + 1) * 6;
			counter++;
		
			wait(NULL);
	end = clock();

		}
			wait(NULL);

	
	}
	fclose(fp);

	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

	printf("\n\nThe elapsed time is %f seconds\n", time_spent);

	return 0;
}