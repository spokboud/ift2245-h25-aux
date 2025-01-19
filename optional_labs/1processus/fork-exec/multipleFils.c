#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main() {
	pid_t pfils;

	for(int i=0 ; i!= 10 ; i++ ){
		pfils = fork();
		if (pfils == -1) {
			perror("Echec du fork");
		} else if(pfils == 0) {
			printf("Je suis le numéro %d, mon PID est %d et mon père est %d ! \n",i+1,getpid(),getppid());
			break;
		}
	}
    // code
	return 0;
}

