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
    int i, indice1, indice2;
    char chemin_pseudo_fichier[20];
    strcpy(chemin_pseudo_fichier, "/proc/self/fd/");

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "{}") == 0) {
            argv[i] = chemin_pseudo_fichier;
            indice1 = i;
        }
        if (strcmp(argv[i], "<|") == 0) {
            argv[i] = NULL;
            indice2 = i + 1;
        }
    }

    int p[2];
    pid_t commande1, commande2;
    pipe(p);

    commande1 = fork();
    if (commande1 == 0) {
        dup2(p[1], 1);
        close(p[1]);
        execvp(argv[indice2], argv + indice2);
        perror(argv[indice2]);
        return 1;
    }

    commande2 = fork();
    if (commande2 == 0) {
        sprintf(argv[indice1], "%s%d", argv[indice1], p[0]);
        close(p[1]);
        execvp(argv[1], argv + 1);
        perror(argv[1]);
        return 1;
    }

    close(p[0]);
    close(p[1]);
    waitpid(commande1, NULL, 0);
    waitpid(commande2, NULL, 0);
    return 0;
}
