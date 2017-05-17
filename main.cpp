#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>
#include<string.h>

static char Red5IP[128];
static char Red5Port[32];

static char * const fileName = "data.conf";
static char * const procName = "ffmpeg";

int NumOfStreams = 1;
static void printMenu(void);
static void printError(char *errMsg);
static pid_t Fork(void);

static char param1[16];
static char input[128];
static char param2[16];
static char codec[128];
static char param3[16];
static char format[128];
static char param4[16];
static char resolution[128];
static char param5[16];
static char output[128];

int main(void)
{
	pid_t pid;
	int ret;
	int status;
	FILE *fd;
	char **argv = (char **)malloc(12*sizeof(char *));
	char **envp = NULL;

       //printf("Input the number of rtmp streams that you want: ");
       //scanf("%d", &NumOfStreams);


	fd = fopen(fileName, "r");
	if(fd == NULL)
	{
		printf("Fail to open %s\n", fileName);
		exit(1);
	}
	if(fscanf(fd, "%d", &NumOfStreams) <= 0) exit(1);
	printf("%d\n", NumOfStreams);
	if(fscanf(fd, "%s", Red5IP) <= 0) exit(1);
	if(fscanf(fd, "%s", Red5Port) <= 0) exit(1);
	printf("%s\n",Red5IP);
	for(int i=1; i<=NumOfStreams; ++i)
	{
   		char param[5];
		ret = fscanf(fd, "%s", param1);
		ret = fscanf(fd, "%s", input);
		ret = fscanf(fd, "%s", param2);
		ret = fscanf(fd, "%s", codec);
		ret = fscanf(fd, "%s", param3);
		ret = fscanf(fd, "%s", format);
		ret = fscanf(fd, "%s", param4);
		ret = fscanf(fd, "%s", resolution);
		ret = fscanf(fd, "%s", param5);
		ret = fscanf(fd, "%s", output);
		if(ret <= 0)
		{
			printf("End of file %s\n", fileName);
			break;
		}

		pid = Fork();
		if(pid == 0)
		{
			argv[0] = procName;
			argv[1] = param1;
			argv[2] = input;
			argv[3] = param2;
			argv[4] = codec;
			argv[5] = param3;
			argv[6] = format;
			argv[7] = param4;
			argv[8] = resolution;
			argv[9] = param5;
			argv[10] = output;

			printf("%s\n",param1);
			ret = execve(procName, argv, envp);
			printf("execve ret = %d\n", ret);
			if(ret < 0)
			{
				printError("Fail to run child process");
			}
			break;
		}
	}

	while(waitpid(-1, &status, 0) > 0);
	fclose(fd);

	return 0;
}

static void printMenu(void)
{
/*
	printf("Input 28181 platform's IP: ");
	scanf("%s", platformIP);
	printf("Input 28181 platform's Port: ");
	scanf("%s", platformPort);
	printf("Input device's login password: ");
	scanf("%s", devicePassword);
*/
	printf("Input the number of rtmp streams that you want: ");
	scanf("%d", &NumOfStreams);
}

static void printError(char *errMsg)
{
	fprintf(stderr, "%s: %s\n", errMsg, strerror(errno));
	exit(0);
}

static pid_t Fork(void)
{
	pid_t pid;
	if ((pid = fork()) < 0)
		printError("fork error");
	return pid;
}
