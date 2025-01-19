#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Vous devez fournir le programme à lancer");
		return 1;
	}
	pid_t pid_fils;
	int wstatut;
	int exit_statut = 0;

	pid_fils = fork();
	if (pid_fils == -1) {
		perror("Echec du fork");
		exit(EXIT_FAILURE);
	} else if(pid_fils == 0) {
		execvp (argv[1], argv+1);
		perror("La commande ne peut être exécutée");
		exit(EXIT_FAILURE);
	}

	pid_fils = waitpid(-1, &wstatut, 0);
	if(pid_fils == -1){
		perror("Erreur de waitpid");
		exit(EXIT_FAILURE);
	}

	if(WIFEXITED(wstatut)) { //retourne "true" si le fils a terminé normalement
		exit_statut = WEXITSTATUS(wstatut); //le retour du processus fils qui a exécuté la commande
		printf("L'exécution de la commande est terminée, avec le code de retour %d.\n", exit_statut);
	}

	exit(exit_statut);
}

