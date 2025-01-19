#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main(){
	pid_t pid_fils;

	printf("Mon PID est <%d>\n", getpid());

	pid_fils = fork();

	if(pid_fils > 0){
		printf("Je suis le père et mon PID est <%d>\n", getpid());
	}
	else if(pid_fils == 0){
		printf("Je suis le fils et mon PID est <%d>\n", getpid());
	}
	else{
		perror("Echec du fork");
	}
}

