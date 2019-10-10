#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(void){
	pid_t pid;
	int data = 10;
	pid = fork();

	if(pid == -1){
		printf("fork fail, process id = %d \n", pid);
		exit(1);
	}
	printf("fork success id : %d \n",pid);

	if(pid == 0){
		printf("child process");
		data+=10;
	} 
	else{
		printf("parent process");
		data-=10;
	}

	printf("data : %d \n\n", data);
	return 0;
}

