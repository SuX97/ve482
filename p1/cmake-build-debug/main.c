#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#define MAX 10240
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
int father_pid;
int current_pid;
int flag_C = 0;
int pidNum = 0;
int pid_store[MAX];
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
int disableReIn[MAX] = {-1};
int disableReOut1[MAX] = {-1};
int disableReOut2[MAX] = {-1}; 
int disablePipe[MAX] = {-1};
int pending1 = 0;
int pending2 = 0;
int pending3 = 0;
int toBreak;


void killAll(){
			for(int j = 0; j<pidNum;j++){
			if(pid_store[j] != -3){
			//waitpid(pid_store[j], NULL, 0);
			kill(pid_store[j],SIGINT);

			pid_store[j] = -3;
		}
			}
}
void handler(){

	int isRunning = 0;
	int current_pid = getpid();
	//printf("The father is %d and the current is %d\n", father_pid,getpid());
	if(current_pid == father_pid){

	for(int i = 0; i < MAX; i++){
		if(pid_store[i] != -3) isRunning = 1;
	}
	if(isRunning){
		printf("\n");
		fflush(stdout);
	}
	else{
		printf("\nmumsh $ ");
		fflush(stdout);
	}
	}
	else{
		exit(0);
	}
}


void shift_2(char* argv[],int index){
	for(int i= index; i<=MAX - 3; i++){
		argv[i] = argv[i+2];
		
}
	argv[MAX -2] = NULL;
	argv[MAX -1] = NULL;


}


void insert(char line[], int index, int flag){
	int i;
	
	if(index >= (int)strlen(line)){
		printf("No!Too long!");
		exit(1);
}	else{
	if (flag == 0){
	for (i = strlen(line); i>= index; i--){
		line[i+1] = line[i];
}
	line[index] = ' ';
	for (i = strlen(line); i>= index + 2; i--){
		line[i + 1] = line[i];
}
	line[index + 2] = ' ';		

	}
	else if(flag == 1){
	for (i = strlen(line); i >= index; i--) line[i+1] = line[i];
	line[index] = ' ';
	for (i = strlen(line); i >= index + 3; i--) line[i+1] = line[i];
	line[index + 3] = ' ';

}
}


}

void parse(char temp[], char *argv[]){

	int i = 0;
	char *p;
	p = strtok(temp," ");
	while(p != NULL){
		argv[i++] = p;
		p = strtok(NULL, " ");
}
	argv[i] = NULL;

}
		

int pwdCmd(){
	char result[255];
	if(!getcwd(result, 255)){
		printf("ERROR IN PWD\n");
		return 1;

	}
	printf("%s\n", result);
	return 0;
}


int cdCmd(char *argv[]){
	if (argv[1] == NULL || argv[2]!=NULL){
		printf("Missing or too many arguments!\n");
		return 1;
	}
	int ret = chdir(argv[1]);
	if(ret == -1){
		printf("%s: No such file or directory\n",argv[1]);
		return 1;
	}
	return 0;
}


void piping(char *argv[],int isBg){
	int inFd = dup(STDIN_FILENO);
	int outFd = dup(STDOUT_FILENO);
	//dup2(inFd,STDIN_FILENO);
	//dup2(outFd,STDOUT_FILENO);
		//if(isBg) fclose(stdout);
		int lastPipe = -1;
		char *argv2[30][30];
		int cntCmd = 0;
		int cntWrd = 0;
		int cntReOut1, cntReOut2, cntPipe,cntReIn;
		cntReOut1 = cntReOut2 = cntPipe = cntReIn = -1;
		for(int i = 0; i< 30 ;i++){
			for(int j = 0; j<30;j++){
				argv2[i][j] = NULL;
			}
		}

		for(int i = 0; argv[i] != NULL ; i++){
			//printf("%s\n", argv[i]);

			if(strcmp(argv[i], "|") == 0){
				cntPipe++;


				if(1){
				//printf("It is not disabled\n");
					cntWrd = 0;
					for(int j = lastPipe + 1; j < i; j++){
						argv2[cntCmd][cntWrd++] = argv[j];
						lastPipe = i;
					}
					argv2[cntCmd][cntWrd] = NULL;
					cntCmd++;
				}
				

			}
		}
			cntWrd = 0;
			for(int j = lastPipe + 1;argv[j]!=NULL ; j++){
				argv2[cntCmd][cntWrd++] = argv[j];
			}

		for(int i = 0; i< 30 ;i++){
			//printf("The %d-th commond!\n", i);
			for(int j = 0; j<30;j++){
				//if(argv2[i][j] != NULL)	printf("%s\n",argv2[i][j]);
			}
		}

		int pid;
		

		int fd[30][2];
		for(int i = 0; i < 30;i++){
			pipe(fd[i]);
		}
		toBreak = 0;
		for(int c = 0; c <= cntCmd; c++){
			//printf("Running the %d-th piped cmd\n",c );
			//printf("The toBreak =%d\n",toBreak);
			if(toBreak == 1){
				break;
			}
			int cntInput = 0;
			int cntOutput = 0;
			pid = vfork();//to share the global variables
			char *comm[30];
			for(int i = 0; i<30;i++) comm[i] = NULL;

			for(int k = 0; k < 30; k++){
				comm[k] = argv2[c][k];
			}

			pid_store[pidNum++] = pid;
			int std_out = dup(STDOUT_FILENO);

			if(pid < 0){
				perror("Fork failed!\n");
				exit(1);
			}
			else if(pid == 0){
			//current_pid = getpid();
			for(int k = 0; k < 30; k++){


//type: 1 for >, 2 for >>, 3 for <, 4 for |, to disable some of the <>|

				if( comm[k] != NULL && (strcmp(comm[k], ">") == 0)){
					cntReOut1 += 1;
					cntOutput += 1;
					if(cntOutput >= 2){
						dup2(std_out,STDOUT_FILENO);
						printf("error: duplicated output redirection\n");
						toBreak = 1;
						exit(0);
					}
					else{

					if(comm[k + 1] == NULL){
						printf("syntax error near unexpected token `|'\n");
						toBreak = 1;
						exit(0);
					}
					else if(
							strcmp(comm[k + 1],">") == 0 || 
							strcmp(comm[k + 1],"<") == 0 ||
							strcmp(comm[k + 1],"|") == 0
							){
						printf("syntax error near unexpected token `%s'\n", comm[k+1]);
						toBreak = 1;
						exit(0);
					}

					int fo1 = open(comm[k + 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
					if(fo1 == -1){
						printf("%s: Permission denied\n", comm[k + 1]);
						toBreak =1;
						exit(0);
					}
					else{
					dup2(fo1,1);
					close(fo1);
					 for(int i = k; i < 28; i++){
						comm[i] = comm[i+2];
					}
					comm[29] = NULL;
					comm[28] = NULL;
					k = k - 2;
					}
				}



				}
				else if( comm[k] != NULL && (strcmp(comm[k], ">>") == 0)){
					cntReOut2++;
					cntOutput += 1;
					if(cntOutput >= 2){
						dup2(std_out,STDOUT_FILENO);
						toBreak = 1;
						printf("error: duplicated output redirection\n");
						exit(0);
					}
					else{
					if(comm[k + 1] == NULL){
						printf("No such file!\n");
						exit(0);
					}
					umask(0000);
					int fo2 = open(comm[k + 1], O_WRONLY | O_APPEND | O_CREAT, 0777);
					dup2(fo2,1);
					close(fo2);
					 for(int i = k; i < 28; i++){
						comm[i] = comm[i+2];
					}
					comm[29] = NULL;
					comm[28] = NULL;
					k = k - 2;
					}



				}
				else if( comm[k] != NULL && (strcmp(comm[k], "<") == 0)){
					cntReIn++;
					cntInput++;
					if(cntInput >= 2){
					dup2(inFd,STDIN_FILENO);
					dup2(outFd,STDOUT_FILENO);
					printf("error: duplicated input redirection\n");
					toBreak = 1;
					exit(0);
				}

					if(comm[k + 1] == NULL){
						printf("No such file!\n");
						exit(0);
					}
					int fi = open(comm[k + 1], O_RDONLY, 0777);
					//printf("The fd is %d\n", fi);
					if(fi == -1) {
						printf("%s: No such file or directory\n", comm[k+1]);
						exit(0);


					}
					else{
					dup2(fi,0);
					close(fi);
					
					 for(int i = k; i < 28; i++){
						comm[i] = comm[i+2];
					}
					comm[29] = NULL;
					comm[28] = NULL;
					k = k - 2;
					}
				}



			}
				if(comm[0] == NULL){

				printf("error: missing program\n");
				dup2(inFd,STDIN_FILENO);
				dup2(outFd,STDOUT_FILENO);
				toBreak = 1;
				exit(0);
				
			}

			if(c > 0){
				cntInput++;
				if(cntInput >= 2){
					printf("error: duplicated input redirection\n");
					exit(0);
				}
				dup2(fd[c - 1][0], STDIN_FILENO);
				close(fd[c - 1][0]);
				close(fd[c - 1][1]);
			}
			if( c < cntCmd ){
				cntOutput += 1;
				if(cntOutput >= 2){
					dup2(std_out,STDOUT_FILENO);
					printf("error: duplicated output redirection\n");
					exit(0);
				}
				else{
				close(fd[c][0]);
				dup2(fd[c][1],STDOUT_FILENO);
				close(fd[c][1]);
			}
			}

			else if(c == cntCmd){
				if(isBg) fclose(stdout);
			}



			if(execvp(comm[0],comm) == -1){
				printf("%s: command not found\n", comm[0]);
				exit(0);
				return;
			};


			}

			else if(pid > 0){
				close(fd[c][1]);
			}
		}

}



int execute(char *argv[]){


	if(strcmp(argv[0],"cd") == 0){
	cdCmd(argv);
	return 0;
	
}
	int isBg = 0;
	for(int i = 0; argv[i] !=NULL; i++){
		if(strcmp(argv[i], "&") == 0 && argv[i+1] == NULL){
			isBg = 1;
		}
	}


	int inFd = dup(STDIN_FILENO);
	int outFd = dup(STDOUT_FILENO);


	piping(argv,isBg);

	dup2(inFd,STDIN_FILENO);
	dup2(outFd,STDOUT_FILENO);

	return 0;
}



int main(){
		char *arglist[MAX];
		father_pid = getpid();

		//printf("The father pid is :%d\n",father_pid );
		int i = 0;
		//signal(SIGINT, SIG_IGN);
		char temp[MAX];
		sighandler_t fp = handler;

		for(int j = 0;j<MAX; j++){

			pid_store[j] = -3;
		}

		while(1){
			int cnt1,cnt2,cnt3,cnt4;
			cnt1 = cnt2 = cnt3 = cnt4 = 0;
			int cntReIn, cntReOut1,cntReOut2,cntPipe;
			cntReIn = cntReOut1 = cntReOut2 = cntPipe = 0;
			signal(SIGINT,fp);
			i = 0;

			for(int j = 0; j<pidNum;j++){
			waitpid(pid_store[j], NULL, 0);
			pid_store[j] = -3;
			}
			memset(disablePipe, -1, MAX);
			memset(disableReIn, -1, MAX);
			memset(disableReOut1, -1, MAX);
			memset(disableReOut2, -1, MAX);

			printf("mumsh $ ");
			fflush(stdout);

			memset(arglist,0,MAX);
			memset(temp,'\0',MAX);

			char ch;


			pending1 = 0;
			pending2 = 0;
			pending3 = 0;

			while(1){
			ch = (char)getchar();
			if(ch == EOF){
				printf("exit\n");
				fflush(stdout);
				return 0;
			}
			else if(ch == '\''){
				if(pending1 == 1 && pending2 == 0){
					pending1 = 0;
					continue;
				}
				else if(pending1 == 0 && pending2 == 0){
					pending1 = 1;
					continue;
				}

			}
			else if(ch == '\"'){
				if(pending1 == 0 && pending2 == 1){
					pending2 = 0;
					continue;
				}
				else if(pending1 == 0 && pending2 == 0){
					pending2 = 1;
					continue;
				}

			}




		/*	else if(ch == '>' || ch == '|' || ch == '<'){
				pending3 = 1;
			}*/
			
			else if(ch != '|' && ch != '<' && ch != '>' && ch != '\n'){
				pending3 = 0;
			}

			else if(ch == ' '){
				if(pending2 == 1|| pending1 == 1){
					//char c= 1;
					temp[i++] = '$';
					continue;
				}

			}

			else if(ch == '>' || ch == '<' || ch == '|'){
				pending3 = 1;
				//printf("Pending1 and 2 is:%d %d\n",pending1,pending2); 
				char test = getchar();
				ungetc(test,stdin);
				if(test == '>'){
					getchar();
					if(pending1 == 1 || pending2 == 1){
						temp[i++] = (char)11;
						continue;
					}
					else{
						temp[i++] = ' ';
						temp[i++] = '>';
						temp[i++] = '>';
						temp[i++] = ' ';
						continue;
					}

				}
				else{
				if(pending1 == 1 || pending2 == 1){
				/*	switch(ch){
						case '>':temp[i++] = (char)12;
						case '<':temp[i++] = (char)13;
						case '|':temp[i++] = (char)14;
					}
					*/
					temp[i++] = ch;

					continue;
				}
				else{

					temp[i++] = ' ';
					temp[i++] = ch;
					temp[i++] = ' ';
					continue;
				}
			}
			}
			

			else if(ch == '\n'){
				if( pending1 == 0 && pending2 == 0 && pending3 == 0){
					fflush(stdout);
					break;
				}
				else if(pending1 == 1 || pending2 ==1 || pending3 == 1){
					temp[i++] = ch;
					printf(">");
					fflush(stdout);
					continue;
				}

			}

			temp[i++] = ch;

			}



			parse(temp,arglist);



			for(int i = 0; arglist[i] !=NULL; i++){
				//printf("%s ", arglist[i]);

				
				for(int j = 0;j < MAX;j++){
				//printf("%c\n", arglist[i][j]);
				if(arglist[i][j] == '$'){
					arglist[i][j] = ' ';
				}
			}

			}


			if(strlen(temp) == 0) continue;
			//flag_C = 1;
			if(strcmp(arglist[0],"exit") == 0){
				printf("exit\n");
				return 0;
}
			execute(arglist);
			//flag_C = 0;

}
}


