//
// Created by guib on 2021-07-20.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char * argv[]) {
    int i, indice2;
    char * nom_tube = "fifo";
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "{}") == 0) {
            argv[i] = nom_tube;
        }
        if (strcmp(argv[i], "<|") == 0) {
            argv[i] = NULL;
            indice2 = i + 1;
        }
    }

    int descripteur;
    pid_t commande1, commande2;
    if (mkfifo(nom_tube, 0666) == -1) {
        perror("mkfifo");
        return 1;
    }

    commande1 = fork();
    if (commande1 == 0) {
        descripteur = open(nom_tube, O_WRONLY);
        if (descripteur == -1) {
            perror("open fichier fifo");
            return 1;
        }
        dup2(descripteur, 1);
        close(descripteur);
        execvp(argv[indice2], argv + indice2);
        perror(argv[indice2]);
        return 1;
    }

    commande2 = fork();
    if (commande2 == 0) {
        execvp(argv[1], argv + 1);
        perror(argv[1]);
        return 1;
    }

    waitpid(commande1, NULL, 0);
    waitpid(commande2, NULL, 0);
    unlink(nom_tube);
    return 0;
}