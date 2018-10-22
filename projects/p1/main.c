#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#define MAX 100
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
//int pidA[MAX];
int flag_C = -1;
//int pIndex = 0;
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);

//char *arglist[MAX];
//int num;
//
void linkHandle(){

}
void handler(int sig){
	//exit(0);
	//printf("Caught Ctrl C!\n");
	//printf("Caught by %d\n",getpid());
	//kill(0,SIGKILL);
	//kill(0, SIGINT);
	//printf("\n");
	//exit(0);
}
int countCmd(char *argv[]){
	int count = 0;
	for(int i = 0;argv[i]!=NULL;i++){
		//printf("The %d-th cmd is %s\n",i,argv[i]);
		count ++;
	}
	return count;

}

void shift_2(char* argv[],int index){
	for(int i= index; i<=MAX - 3; i++){
		argv[i] = argv[i+2];
		
}
	argv[MAX -2] = NULL;
	argv[MAX -1] = NULL;


}

void redir_o(char *file,int flag){
	//printf("Redirect to %s\n.", file);
	int fd = 0;
	if(flag == 0){
	fd = open(file, O_WRONLY | O_CREAT |O_TRUNC, 0777);
}
	else if(flag == 1){
	fd = open(file, O_RDWR | O_APPEND | O_CREAT, 0777);

}

	dup2(fd,STDOUT_FILENO);
	close(fd);


}

void redir_i(char *file){
	freopen(file,"r",stdin);
}

void insert(char line[], int index, int flag){
	int i;
	
	if(index >= strlen(line)){
		printf("No!Too long!");
		exit(1);
}	else{
	if (flag == 0){				//> or <
	for (i = strlen(line); i>= index; i--){
		line[i+1] = line[i];
}
	line[index] = ' ';
	for (i = strlen(line); i>= index + 2; i--){
		line[i + 1] = line[i];
}
	line[index + 2] = ' ';		

	}
	else if(flag == 1){			//>>
	for (i = strlen(line); i >= index; i--) line[i+1] = line[i];
	line[index] = ' ';
	for (i = strlen(line); i >= index + 3; i--) line[i+1] = line[i];
	line[index + 3] = ' ';

}
}

//printf("after insert:%s\n", line);

}

void parse(char temp[], char *argv[]){
	//temp[strlen(temp) - 1]='\0';

/****Add blank to > and >>****/
	char *ptr;
	ptr = strchr(temp, '>');
	while(ptr != NULL){
	if (temp[ptr-temp+1] != '>'){
		insert(temp ,ptr - temp,0);
		
		ptr = strchr(ptr + 2, '>');
}
	else if(temp[ptr - temp + 1] == '>'){
		insert(temp, ptr - temp, 1);
		ptr = strchr(ptr + 3, '>');
}
	else{
	printf("Fuck, wrong!\n");
	//exit(1);
}

}
/****Add blank to <****/
	char *ptr2;
	ptr2 = strchr(temp, '<');
	while(ptr2 != NULL){
	//printf("< is at:%d\n", ptr2 - temp);
	insert(temp, ptr2 - temp, 0);
	ptr2 = strchr(ptr2 + 2, '<');
}

/****Add blank to |****/
	char *ptr3;
	ptr3 = strchr(temp, '|');
	while(ptr3 != NULL){
	//printf("< is at:%d\n", ptr2 - temp);
	insert(temp, ptr3 - temp, 0);
	ptr3 = strchr(ptr3 + 2, '|');
}

/****Seperate with blank****/
	int i = 0;
	char *p;
	p = strtok(temp," ");
	while(p != NULL){
		argv[i++] = p;
		p = strtok(NULL, " ");
}
	argv[i] = NULL;

	//printf("Seperated by blank!\n");
}
		


void dealRedir(char *argv[]){
	int i;
	for(i = 0; argv[i]!=NULL;i++){
	if( strcmp(argv[i], "<") == 0){
		if(argv[i+1] ==NULL){
			printf("No file.\n");
			exit(0);
}
		else{
			redir_i(argv[i+1]);
			shift_2(argv,i);

			i-=1;
}
}	

	else if(strcmp(argv[i], ">") == 0){
		if(argv[i+1] ==NULL){
			printf("No file.\n");
			exit(0);
}
		else{
			redir_o(argv[i+1],0);
			shift_2(argv,i);
			i-=1;
}
}

	else if(strcmp(argv[i], ">>") == 0){
		if(argv[i+1] ==NULL){
			printf("No file.\n");
			exit(0);
}
		else{
			redir_o(argv[i+1],1);
			shift_2(argv,i);
			i-=1;
}

}
}
}

int pwdCmd(){
	char result[255];
	if(!getcwd(result, 255)){
		return 1;
		printf("ERROR IN PWD\n");

	}
	printf("%s\n", result);
	return 0;
}


int cdCmd(char *argv[]){
	if (argv[1] == NULL || argv[2]!=NULL){
		printf("Missing or too many arguments!\n");
		return 1;
	}
	//printf("Dir is:%s!\n", argv[1]);
	int ret = chdir(argv[1]);
	if(ret == -1){
		printf("Cd ERROR!\n");
		return 1;
	}
	return 0;


}



int callRe(char *argv[]){
	pid_t pid;
	//int childId;
	pid = fork();

	//childId = getpid();
	if(pid == 0){
			//printf("Redirect: Child: %s, with PID = %d\n", argv[0],getpid());
			//pidA[pIndex] = childId;
			//pIndex += 1;
			//signal(SIGINT,handler);

			dealRedir(argv); //Dealing with redirct: redirect and shift_2
/****execute!****/
			if(strcmp(argv[0], "pwd") == 0){
			pwdCmd();
			exit(0);
			return 0;
			}
			else{
				if(feof(stdin)){
					printf("STDIN is eof in the child process!\n");
				}
				if(execvp(argv[0],argv)){
				printf("no such file or directory\n");
				}
				exit(0);
				}
		
}

	else if (pid > 0){
		//signal(SIGINT,handler);

		//printf("Redirect: Father: %s, with PID = %d\n", argv[0],getpid());
		wait(NULL);
		return 0;
}

//Fail to create process
	else{
	printf("Process creation error\n!");
	return 1;
}
	return 0;

}


int callPi(char *argv[]){
	int index = -1;
	for(int i = 0; argv[i] != NULL; i++){
		if(strcmp(argv[i], "|") == 0){
			//printf("| found at %d\n", i);
			index = i;
			argv[i] = NULL;
			break;
		}
	}
	if(index == -1){
		callRe(argv);
		return 0;
	}
	else if(argv[index + 1] == NULL){
		printf("No cmd after pipe!\n");
		return 0;
	}

	char *comm[MAX];

	for(int j = 0; j<= index; j++){
		comm[j] = argv[j];
	}

	int fd[2];
	pid_t pid;

	if(pipe(fd) == -1){
		return 1;
	}
	pid = fork();

	if(pid == -1){

		printf("Fail to fork!\n");
		return 1;
	}
	if(pid == 0){
		//printf("Pipe: child: %s!\n",comm[0]);
		close(fd[0]);
		close(1);
		dup2(fd[1], 1);
		close(fd[1]);
		callRe(comm);//The single cmd
		exit(0);		
	}
	else if(pid > 0){

		//printf("Pipe: father:%s!\n",argv[index+1]);
		close(fd[1]);
		close(0);
		dup2(fd[0],0);
		close(fd[0]);
		//waitpid(pid, &status, 0);
		callPi(&argv[index+1]);//Next cmd
		wait(NULL);
	}

	return 0;
}

int execute(char *argv[]){

/*	if(strcmp(argv[0],"exit") == 0){
	printf("exit\n");
	return 0;
}*/
	if(strcmp(argv[0],"cd") == 0){
	//printf("Go to %s!\n", argv[1]);
	cdCmd(argv);
	return 0;
	
}

	int inFd = dup(STDIN_FILENO);
	int outFd = dup(STDOUT_FILENO);


	callPi(argv);

	dup2(inFd,STDIN_FILENO);
	dup2(outFd,STDOUT_FILENO);

return 0;
}


int main(){
	char *arglist[MAX];
		//char* temp = (char*)malloc(1024);
		//char victim;
		int i = 0;
		//signal(SIGINT, SIG_IGN);
		char ch;
		char temp[MAX];

		//sighandler_t fp = handler;
		//signal(SIGINT,fp);
		while(1){

			i = 0;

			printf("mumsh $ ");
			fflush(stdout);

			/*while((temp[i] = getchar())!='\n'){
				i++;
			}*/
			//getchar();
			memset(arglist,0,MAX);
			memset(temp,0,sizeof(temp));
			while(1){
			ch = (char)getchar();


			if(ch == '\n'){//if pressed enter
				break;
				
			}

			else if(ch == EOF){//if reach the end
				if(arglist[0] == NULL){
					printf("exit\n");
					return 0;
				}

			}
			else{
				temp[i++] = ch;
				
				}
			}

			
			parse(temp,arglist);

			if(strlen(temp) == 0) continue;

			if(strcmp(arglist[0],"exit") == 0){
				printf("exit\n");
				return 0;
}
			execute(arglist);

}
			//free(buff);
}

//}
