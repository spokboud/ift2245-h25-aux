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

