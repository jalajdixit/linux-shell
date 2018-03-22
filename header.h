
/* Header files */

#ifndef _SHELL_HEADER
#define _SHELL_HEADER

#include<time.h>
#include<utime.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<dirent.h>
#include<strings.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/resource.h>
#include"colors.h"

void hist(void);
void add_history(char*);
void cmdexec(char*);
void help(void);

int seq,count;

typedef struct hist
{
	int srno;
	char cmdstr[130];
	struct hist *next;
} HIST;
HIST *hp;

#endif
