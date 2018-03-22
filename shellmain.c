
/* Main for jshell */

#include"header.h"

void main(int argc,char *argv[])
{

	if(argc!=1)
	{
		if(strcmp(argv[1],"-c"))
		{	printf("jshell: %s is not an option\n",argv[1]);
			return;
		}
		execvp(argv[2],&argv[2]);
	}

	system("clear");
	signal(2,SIG_IGN);
	seq=1;
	char *login=getlogin();
	char cmd[129],*path;
	while(1)
	{
		int i,co;
		path=getcwd(0,0);
		for(i=0,co=0;path[i];i++)
		{
			if(path[i]=='/')
			co++;
			if(co==3)
			break;
		}

		printf(CYAN BOLD"<%d %s>"RESET":~"GREEN BOLD"%s"RESET"$ ",seq,login,&path[i]);
		gets(cmd);
		cmd[128]='\0';
		free(path);

		if(!cmd[0] || cmd[0]=='^')
		continue;

		add_history(cmd);
		seq++;
	
		if(!strcmp(cmd,"curpid"))
		printf("Pid = %d\n",getpid());
		else if(!strcmp(cmd,"ppid"))
		printf("Parent Pid = %d\n",getppid());
		else if(!strcmp(cmd,"quit"))
		exit(0);
		else if(!strcmp(cmd,"hist"))
		hist();
		else if(!strcmp(cmd,"help"))
		help();
		else
		cmdexec(cmd);
	}
}
