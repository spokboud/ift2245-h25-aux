#### Mise en bouche
```c
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

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
```
#### Multiple fils
#### Programme p1.c
```c
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
	return 0;
}
```
#### Programme p1 avec getchar
```c
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
```
#### Même si un processus fils termine son execution, il garde son entrée dans la table des processus jusqu’à ce que le processus père termine ou fait un appel waitpid, entre temps le processus fils prend l’état zombie, on peut le visualiser en éxecutant la commande ps -al, l’option -l permet d’afficher l’état des processus.
#### Programme p1 avec waitpid
```C
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	pid_t pfils[10], pfils_tmp;
	int wstatus, i, j;

	for(i=0 ; i!= 10 ; i++ ){
		pfils[i] = fork();
		if (pfils[i] == -1) {
			perror("Echec du fork");
		} else if(pfils[i] == 0) {
			printf("Je suis le numéro %d, mon PID est %d et mon père est %d ! \n",i+1,getpid(),getppid());
			return 0 ;
		}
	}

	for(i=0 ; i!= 10 ; i++ ){
		pfils_tmp = waitpid(-1,&wstatus,0);
		if(pfils_tmp == -1){
			perror("Erreur de waitpid");
		}else{
			for(j=0;j!=10;j++) {
				if(pfils[j] == pfils_tmp){
					printf("Le processus No. %d, PID %d vient de terminer.\n",j+1,pfils_tmp);
				}
			}
		}
	}
	getchar();
	return 0;
}
```
#### L’appel système waitpid nous a permis de suspendre l’execution du programme père et d’attendre jusqu’à ce que le processus fils termine son execution, et aussi de libérer ses ressources et de l’enlever de la table des processus.
#### Remarque. L’appel système waitpid(-1 &wstatus, 0) attend la fin de n’importe lequel des fils du processus appelant (comportement dû entre autre à la valeur -1 du paramètre pid), noter qu’on aurait très bien pu utiliser l’appel système wait à la place (wait(&wstatus)) qui a exactement le même comportement).

#### Hiérarchie de processus
#### Programme pchaine.c
```c
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Vous devez fournir le nombre de processus enfants à lancer");
		return 1;
	}
	char* endptr = NULL;
	long i, n = strtol(argv[1], &endptr, 0);
	if (*endptr != '\0' || n <= 0) {
		fprintf(stderr, "Le nombre de processus doit être un nombre entier supérieur à 0\n");
		return 1;
	}
	int wstatus;
	pid_t pfils;

	printf("Je suis le père, mon PID est <%d>\n", getpid());

	for(i=0 ; i<n ; i++){
		pfils = fork();
		if(pfils == 0){
			printf("\tJe suis un fils, mon PID est <%d> et mon père est <%d>\n",  getpid(), getppid());
			return 0;
		}
	}

	for(i=0 ; i<n ; i++){
		pfils = waitpid(-1,&wstatus,0);
		if(pfils == -1){
			perror("Erreur de waitpid");
		}
	}
	return 0;
}
```
#### Programme parbre.c
```c
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Vous devez fournir le nombre de processus enfants à lancer");
		return 1;
	}
	char* endptr = NULL;
	long i,j, n = strtol(argv[1], &endptr, 0);
	if (*endptr != '\0' || n <= 0) {
		fprintf(stderr, "Le nombre de processus doit être un nombre entier supérieur à 0\n");
		return 1;
	}
	int wstatus;
	pid_t pfils;

	printf("Je suis le père, mon PID est <%d>\n", getpid());

	for(i=0; i<n; i++){
		pfils = fork();
		if(pfils == 0) {
			for(j=0; j<i; j++){
				printf("\t");
			}
			printf("\tJe suis un fils, mon PID est <%d> et mon père est <%d>\n",  getpid(), getppid());
			if(i == n-1){ //on termine le dernier processus fils ici pour éviter qu'il éxecute waitpid vue qu'il n'a pas de fils à attendre
				return 0;
			}
		}
		else{
			break;
		}
	}
	pfils = waitpid(-1,&wstatus,0);
	if(pfils == -1){
		perror("Erreur de waitpid");
	}

	return 0;
}
```
#### Programme mystère
#### On a un processus père et 10 processus fils
#### Hiérarchie des processus :


#### Programme mystere.c avec affichage du PID et du PID parent
```c
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void) {
    if (fork()*fork())
    	fork();
    else
    	fork() || fork();

    printf("Mon PID est %d et mon père est %d ! \n",getpid(),getppid());
    int wstatus;
	// Tant que waitpid retourne le PID d'un des porcessus enfants qui termine, continuer la boucle
    while(waitpid(-1,&wstatus,0) != -1);
    return 0;
}
```
#### Programme mystere.c avec output en format graphviz
```c
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(void) {
    pid_t pidPere ;
    pidPere = getpid() ;
    int wstatus ;
    printf("digraph { \nrankdir = BT\n");
    fflush(stdout);

    if (fork()*fork())
    	fork();
    else
    	fork() || fork();

 	if(pidPere != getpid()){
    	printf("%d -> %d\n",getppid(),getpid());
    }

	while(waitpid(-1,&wstatus,0) != -1);

    if(pidPere == getpid()){
    	printf("}");
    }
    return 0;
}
```
#### Recouvrement
#### L’intérpreteur de commandes est un programme executé par un processus, lorsqu’on lance la commande exec, elle va écraser la zone mémoire de ce processus(où il y a le segment de code de l’intérpreteur) par celui de la commande ps. Donc, l’interpreteur se ferme vue qu’il n y a plus de programme qui va l’exécuter.
#### Programme exec0.c
```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Vous devez fournir le programme à lancer");
		return 1;
	}
	execlp(argv[1], argv[1], (char *)NULL);
	perror("La commande ne peut pas être exécutée");
	exit(EXIT_FAILURE);
}
Programme exec0.c avec execvp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Vous devez fournir le programme à lancer");
		return 1;
	}
	execvp (argv[1], argv+1);
	perror("La commande ne peut pas être exécutée");
	exit(EXIT_FAILURE);
}
```
#### Programme exec0 avec affichage de la fin de l’exécution
```c
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
```
#### Programme exec0.c avec execv
```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int exec_avec_chemin(char *nom , char **argv){
	char *path = getenv("PATH");
	if (path == NULL) {
		fprintf(stderr, "Pas de PATH défini\n");
		return 1;
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
	return 1;
}

int main(int argc, char **argv) {
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
```
#### Programme exec5.c
```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
	long compteur ;
	char str_compteur[2];
	if(argc == 1) {
		compteur = 5 ;
		sprintf(str_compteur, "%ld", compteur);
	}
	else{
		char* endptr = NULL;
		compteur = strtol(argv[1], &endptr, 0);
		if (*endptr != '\0' || compteur > 5 || compteur < 1) {
			fprintf(stderr, "Le compteur doit être un nombre entier positif inférieur à 5\n");
			return 1;
		}
		sprintf(str_compteur, "%ld", --compteur);
	}

	if(compteur > -1){
		if(compteur < 5){
			printf("Je suis le recouvrement %ld\n",5-compteur);
		}

		execl(argv[0], argv[0],str_compteur,NULL);
		perror("Echec de l'exec");
		exit(EXIT_FAILURE);
	}
	return 0;
}
```