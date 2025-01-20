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
			return 0;
		}
	}
	getchar();
	return 0;
}

/*
 * Même si un processus fils termine son execution, il garde son entrée dans la table des processus
 * jusqu'à ce que le processus père termine (processus init hérite des enfants et fait les wait()) ou
 * fait un appel waitpid, entre temps, le processus fils prend l'état zombie, on peut le visualiser en
 * éxecutant la commande ps -al, l'option -l permet d'afficher l'état des processus.
 */
