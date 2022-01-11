#include <stdio.h>
#include<sys/wait.h>
#include <memory.h>
#include <stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<ctype.h>
#include <stdbool.h>
#include <pthread.h>
#include <readline/readline.h>
#include <readline/history.h>


#include<unistd.h>



#define MAX_BUFF_SIZE 1024
static char* args[MAX_BUFF_SIZE];

static int fd[2];
static int fd_new = 0;
static int pid;
static int ind = 0;
static int ret;

static pthread_mutex_t mutex;
static pthread_cond_t cond;

static int flag = 0;

void reset()
{
	ind = 0;
	fd_new = 0;
	fd[0] = 0;
	fd[1] = 0;
	args[0] = 0;
}


void * thread_handle(void * param)
{
	while(1) {
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		if( 1  ==   flag ) {
			exit(0);
		}
		flag = 0;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}





void executeCommands(char *args[], int pipesCount,int k) 
{
	ret = pipe(fd);
	if(ret == -1){
		printf("Unable to pipe\n");
		return;
	}
	pid = fork();
	if(pid == -1)
	{
		exit(1);
	}
	else if (pid  == 0)					// Child Process
	{

		dup2(fd_new ,0);			// taking the STDIN and replace it with fd_new.
		if(ind < pipesCount){			// if there are pipes ( or remaining pipes ) then the STDOUT now is the "write" hand side of the the pipe.
			dup2(fd[1],1);
		}
		close(fd[0]);
		/* loop to store file discriptor names */
		for(int j = 0;j<k;j++)
		{
			/* args now should be without spaces and without '|' */
			if(strcmp(args[j],">")  == 0) 
			{
				args[j] = NULL; 	// makeing '>' null.
				j = j+1; 			// inding the first char of file name.

				close(1);		//closing the STDOUT.

				char fn[MAX_BUFF_SIZE];	//making new buffer to store the file name.
				strncpy(fn,args[j],99);		//storing the file into that buff.

				int lfd=open(fn, O_RDONLY);
				
				if(lfd==-1)
				{
					open(fn, O_CREAT | O_RDWR | O_TRUNC);
				}
				args[j] = NULL;
			}
			else if(strcmp(args[j],">>")  == 0)			// appeinding to the file descriptor.
			{
				args[j] = NULL;
				j = j+1;
				
				close(1);

				char fn[MAX_BUFF_SIZE];
				strncpy(fn,args[j],99);

				int lfd=open(fn, O_RDONLY);
				
				if(lfd==-1)
				{
					open(fn, O_CREAT | O_RDWR | O_TRUNC);
				}
				args[j] = NULL;
			}			
		}

		ret = execvp(args[0], args);		//executeing the command line with its arguments.
		if(ret == -1)
		{
			printf("Unable to execute\n");	
			exit(0);	
		}
	}
    else				// Parent Process
    {
		wait(NULL);
		close(fd[1]);
		fd_new = fd[0];
     
    }
}




/* MACRO TO STORE LINES */

#define PRINTLINE \
	FILE * file = fopen("h.txt", "a");\
	if(NULL == file)\
	{ \
	printf("Unable to open");\
	return;\
	}\
	fputs(command, file);\
	fputc('\n', file);\
	fclose(file);\
	lastLineIndex++;\
	currentLineIndex = lastLineIndex;

/* END OF MACRO */








void shell_loop()
{
		int flag=1;
		int lastLineIndex = 1;
		int currentLineIndex = 0;
		int q = 0;
		char *ch;
		do
		{
			char command[MAX_BUFF_SIZE];		//stack allocated char array to store the scaned command line in it.
			printf("AHMAD$>");			// printing the init prompt to show for the user that the shell is ready to take commands.
			fflush(stdin);
			scanf("%[^\n]%*c",command);		// scan from the user input string includeing the spaces ( "%[^\n]  means that the spcaes are not excluded")
			// while(1)
			// {
			// 	if(q + 1 >= MAX_BUFF_SIZE)
			// 	{
			// 		printf("your command reached the max size: 1024 characters \n");
			// 	}
			// 	ch = getchar();
			// 	if(ch == '\n')
			// 	{
			// 		command[q] = '\0';
			// 		q = 0;
			// 		fflush(stdin);
			// 		break;
			// 	}
			// 	command[q] = ch;
			// 	q++;
			// }
			// PRINTLINE;
			const int commandLength = strlen(command);			// length of the inputed command.
			int pipesCount = 0;					
			for(int i = 0;i < commandLength;i++){						// loop to count the number of pipes '|'.
				if(command[i]  == '|'){
					pipesCount++;			
				}
			}

			char pipeCommands[pipesCount+1][100];					// 2D stack allocated array to store every commends between pipes ( if pipes were in the user inputs )
			int j = 0,k = 0;
			for(int i = 0;i < commandLength ;i++){ 			// loop to replace the '|' char with null terminator to make sure that the pipeCommand with the current ind is terminated currectly.
				if(command[i] == '|'){
					pipeCommands[j][k-1] = '\0';
					j++;
					k = 0;									
				}
				else if(command[i] == ' '){				// if the current char is empty space then it replaced with spicial char to use it later with another separate mechanisim.
					pipeCommands[j][k] = '$';
					k++;			
				}
				else{
					pipeCommands[j][k] = command[i];
					k++;
	
				}			
			}
			pipeCommands[j][k] = '\0';					// terminate the pipeCommand
			

			for(int i = 0; i<pipesCount+1;i++)			// Loop for each pipe sequence... the commands at this moment should be:
																		// example1: pipeCommands[][] = { "ls$-l$\0", "grep$-e$'tmp'$>$out.txt\0"} , was "ls -l | grep -e 'tmp' > out.txt"
																		//example2: pipeCommands[][] = { "ls$-l$\0", "grep$-e$'tmp'\0", "wc$-l\0"} , was "ls -l | grep -e 'tmp' | wc -l"
			{
				// printf("Pipe command : %s\n", pipeCommands[i]);
				char *args[10];	
				int k = 0;
				if(i == 0){								// First command doesn't has starting symbol $
					args[0] = &pipeCommands[i][0];
					k++;
				}

				unsigned int size = strlen(pipeCommands[i]);
				for(int j = 0;j<size;j++)
				{
					if(pipeCommands[i][j] == '$'){
							pipeCommands[i][j] = '\0';
							args[k] = &pipeCommands[i][j+1];
							k++;			
					}
				}
				args[k] = NULL;

				if(strcmp(args[0], "exit") == 0)
				{
					exit(0);
				}

				executeCommands(args, pipesCount,k);
				ind++;
			}

			reset();	

	}while(flag == 1);
}




int main(){
	shell_loop();
	return 0;
}