#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

void exec_avec_chemin(char *nom , char **argv){
	char *path = getenv("PATH");
	if (path == NULL) {
		fprintf(stderr, "Pas de PATH défini\n");
		return;
	}
	char *ptr = strtok(path, ":");
	char *potentiel_path;

	while(ptr != NULL)
	{
		potentiel_path = malloc(strlen(ptr) + strlen(nom) + 2);
		strcpy(potentiel_path, ptr);
		strcat(potentiel_path, "/");
		strcat(potentiel_path, nom);

		execv(potentiel_path, argv);

		ptr = strtok(NULL, ":");
	}
}

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "aucun argument\n");
        exit(1);
    }
	pid_t pid_fils;
	int wstatut;
	int exit_statut = 0;

	pid_fils = fork();
	if (pid_fils == -1) {
		perror("Echec du fork");
		exit(EXIT_FAILURE);
	} else if(pid_fils == 0) {
		exec_avec_chemin(argv[1], argv+1);
		perror("La commande ne peut être exécuté");
		exit(EXIT_FAILURE);
	}

	pid_fils = waitpid(-1, &wstatut, 0);
	if(pid_fils == -1){
		perror("Erreur de waitpid");
		exit(EXIT_FAILURE);
	}

	if(WIFEXITED(wstatut)) {
		exit_statut = WEXITSTATUS(wstatut);
		printf("\nL'exécution de la commande est terminée, avec le code de retour %d.\n", exit_statut);
	}
	exit(exit_statut);
}

