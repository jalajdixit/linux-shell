
/* Hist function for history */

#include"header.h"
void hist(void)
{
	HIST *p=hp;
	while(p)
	{
		printf("%2d %s\n",p->srno,p->cmdstr);
		p=p->next;
	}
}

void add_history(char *cmd)
{
	static HIST *last;
	HIST *new;

	if(hp && !strcmp(cmd,last->cmdstr))
	{ seq--; return; }

	new=malloc(sizeof(HIST));
	new->srno=seq;
	strcpy(new->cmdstr,cmd);
	new->next=0;
	count++;

	if(count==11)
	{
		HIST *temp;
		temp=hp;
		hp=hp->next;
		free(temp);
		count--;
	}

	if(hp==0)
	last=hp=new;
	else
	last=last->next=new;
}

void help(void)
{
	printf(GREEN BOLD"\n\t\tjshell"RESET" help section\n");
	printf(BLUE BOLD"\tcurpid"RESET": show pid of process\n");
	printf(BLUE BOLD"\t  ppid"RESET": show parent process id\n");
	printf(BLUE BOLD"\tcd Dir"RESET": change the directory\n");
	printf(BLUE BOLD"\t  hist"RESET": show history of 10 commands\n");
	printf(BLUE BOLD"\t  help"RESET": show help section of jshell\n");
	printf(BLUE BOLD"\t  quit"RESET": exit from jshell\n");
	printf(GREEN BOLD"\n\tjshell"RESET" supports,\n\t\t- multiple commands"BOLD" (;)"RESET);
	printf("\n\t\t- input redirection"BOLD" (<)"RESET"\n\t\t- output redirection"BOLD" (>)"RESET);
	printf("\n\t\t- pipeline mechanism"BOLD" (|)\n"RESET);

	printf(YELLOW BOLD"\n\tjshell"RESET" designed by "RED BOLD"Jalaj Dixit\n"RESET);
	printf("\tEmail"RESET":"BOLD" jalajdixit1@gmail.com \n\n"RESET);
}
