#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Vous devez fournir le programme à lancer");
		return 1;
	}
	execlp(argv[1], argv[1], (char *)NULL);
	perror("La commande ne peut pas être exécutée");
	exit(EXIT_FAILURE);
}

