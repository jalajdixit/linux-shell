
/* Execute Standard Commands */

#include"header.h"

void cmdexec(char *cmd)
{
	int semi=0;
	char **list;

	if(strchr(cmd,';')) 		//Semicolon ;
	{
		int i;
		for(i=0,semi=1;cmd[i];i++)
		if(cmd[i]==';')
			semi++;
		list=malloc(sizeof(char*)*semi);
		list[0]=strtok(cmd,";");
		for(i=1;i<semi;i++)
		if(!(list[i]=strtok(NULL,";")))
			break;
	}
	else
	{
		list=malloc(sizeof(char*));
		list[0]=cmd;
	}
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
	{				//Normal
		int len,co,j,k,i=0;
		int out,in,flp;	
		char **str,*inout;
		do
		{
			out=in=flp=0;
			if(strchr(list[i],'>'))		//Output >
			{
				out=1;
				dup2(1,5);	//keeping copy of STDOUT at 5
				close(1);	//closing STDOUT

				inout=strtok(list[i],">");
				inout=strtok(NULL,">");
				for(j=0;inout[j];j++)
				if(inout[j]==' ')		//removing spaces from filename
				{
					for(k=j;inout[k];k++)
					inout[k]=inout[k+1];
					j--;
				}

				open(inout,O_WRONLY|O_CREAT|O_TRUNC,0644);

				for(k=0;list[i][k];k++)
				if(list[i][k]=='>')		//getting command & removing > 
				{
					list[i][k]='\0';
					break;
				}
			}
			else if(strchr(list[i],'<'))	//Input <
			{
				in=1;
				dup2(0,7);	//keeping copy of STDIN at 7
				close(0);	//closing STDIN

				inout=strtok(list[i],"<");
				inout=strtok(NULL,"<");
				for(j=0;inout[j];j++)
				if(inout[j]==' ')		//removing spaces from filename
				{
					for(k=j;inout[k];k++)
					inout[k]=inout[k+1];
					j--;
				}

				if(open(inout,O_RDONLY,0)<0)
				{
					char er[50];
					strcpy(er,"jshell: ");
					strcat(er,inout);
					perror(er);
					dup2(7,0);	//opening STDIN
					continue;
				}

				for(k=0;list[i][k];k++)
				if(list[i][k]=='>')		//getting command & removing <
				{
					list[i][k]='\0';
					break;
				}
			}
			
			if(strchr(list[i],'|'))			//Pipeline Mechanism
			{
				flp=1;
				if(in)
				{
					printf("jshell: cannot use < (input redirection) with | (pipeline)\n");
					dup2(7,0);
					continue;
				}

				int pcol=0,ccol=0;
				char *par_cmd,*cld_cmd;			//Seperating Parent Child Commands
				par_cmd=strtok(list[i],"|");
				cld_cmd=strtok(NULL,"|");

				//Detecting Color Outputs
				if(strstr(par_cmd,"ls ") || strstr(par_cmd,"grep ")) pcol=1;
				if(strstr(cld_cmd,"ls ") || strstr(cld_cmd,"grep ")) ccol=1;

				char **parent,**child;
				int pc,cc,a,b,c,x;
				for(a=0,pc=0;par_cmd[a];a++)		//Removing Spaces Parent
				{
					if(par_cmd[a]!=' ' && (par_cmd[a+1]==' ' || par_cmd[a+1]=='\0'))
						pc++;
					if(par_cmd[a]==' ')
						par_cmd[a]='\0';
				}
				for(b=0,cc=0;cld_cmd[b];b++)		//Removing Spaces Child
				{
					if(cld_cmd[b]!=' ' && (cld_cmd[b+1]==' ' || cld_cmd[b+1]=='\0'))
						cc++;
					if(cld_cmd[b]==' ')
						cld_cmd[b]='\0';
				}

				if(pcol) pc++;
				parent=malloc(sizeof(char*)*pc+1);	//making 2D array for parent commands
				if(par_cmd[0]!='\0')
				{
					parent[0]=par_cmd;
					c=1;
				}
				else c=0;
				for(x=1;x<a;x++)
				if(par_cmd[x]!='\0' && par_cmd[x-1]=='\0')
					parent[c++]=&par_cmd[x];
				if(pcol) parent[c++]="--color=always";
				parent[c]=NULL;

				if(ccol) cc++;
				child=malloc(sizeof(char*)*cc+1);	//making 2D array for child commands
				if(cld_cmd[0]!='\0')
				{
					child[0]=cld_cmd;
					c=1;
				}
				else c=0;
				for(x=1;x<b;x++)
				if(cld_cmd[x]!='\0' && cld_cmd[x-1]=='\0')
					child[c++]=&cld_cmd[x];
				if(ccol) child[c++]="--color=always";
				child[c]=NULL;


				if(fork()==0)
				{
				int pi[2];	//making Pipeline
				pipe(pi);
				dup2(0,10);	//copying IN
				dup2(1,11);	//copying OUT
					if(fork()==0)
					{	dup2(pi[1],1);
						close(pi[0]);
						if(execvp(parent[0],parent)<0)
						{
							printf("jshell: %s: command not found\n",parent[0]);
							raise(9);
						}
					}
					else
					{
						dup2(pi[0],0);
						close(pi[1]);
						wait(0);
						if(execvp(child[0],child)<0)
						{
							printf("jshell: %s: command not found\n",child[0]);
							raise(9);
						}
					}
				}
				else wait(0);
			/*	dup2(10,0);	//reseting IN
				dup2(11,1);	//reseting OUT
				close(10); close(11);
			*/	goto skip;
			}
/*----------------------------------------------------------------*/

			for(co=0,len=0;list[i][len];len++)	//replacing spaces from commands with \0
			{
				if(list[i][len]!=' ' && (list[i][len+1]==' ' || list[i][len+1]=='\0'))
					co++;
				if(list[i][len]==' ')
				list[i][len]='\0';
			}
			str=malloc(sizeof(char*)*co+1);		//making an array of pointer to store all commands
			if(list[i][0]!='\0')
			{
				str[0]=list[i];
				k=1;
			}
			else k=0;

			for(j=1;j<len;j++)
			if(list[i][j]!='\0' && list[i][j-1]=='\0')
				str[k++]=&list[i][j];
			str[k]=NULL;

			if(!strcmp(str[0],"cd"))		//cd command
			{
				if(str[1]==NULL)
				str[1]="~/";
				if(chdir(str[1])<0)
				perror("jshell: cd");
			}
			else					//execute commands using execvp
			{
				if(fork()==0)
				{
					if(execvp(str[0],str)<0)
					{
						printf("jshell: %s: command not found\n",str[0]);
						raise(9);
					}
				}
				else wait(0);
			}

			free(str);
		skip:	i++;

			if(out)
			dup2(5,1);	//opening STDOUT
			if(in)
			dup2(7,0);	//opening STDIN
		}while(i<semi);
	}
}
