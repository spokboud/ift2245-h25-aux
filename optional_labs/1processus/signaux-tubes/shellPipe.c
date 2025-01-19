//
// Created by guib on 2021-07-20.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char ** argv) {
    /*on fera confiance aux arguments entrés par l'utilisateur(pour cette fois-ci)*/
    int indice_debut = 0;
    for (int i = 1; i != argc; i++) {
        if (strcmp(argv[i], "+") == 0) {
            argv[i] = NULL;
            indice_debut = i + 1;
            break;
        }
    }
    int p[2];
    /***************************************************
     * p[0] refers to the read end of the pipe.   *
     * p[1] refers to the write end of  the pipe. *
     ***************************************************/
    pipe(p);
    pid_t commande1 = fork();
    if (commande1 == 0) {
        dup2(p[1], 1);
        close(p[0]);
        close(p[1]);
        execvp(argv[1], argv + 1);
        perror(argv[1]);
        return 1;
    }
    pid_t commande2 = fork();
    if (commande2 == 0) {
        dup2(p[0], 0);
        close(p[0]);
        close(p[1]);
        execvp(argv[indice_debut], argv + indice_debut);
        perror(argv[indice_debut]);
        return 1;
    }
    close(p[0]);
    close(p[1]);
    int wstatus;
    waitpid(commande1, NULL, 0);
    waitpid(commande2, &wstatus, 0);
    if (WIFEXITED(wstatus)) {
        return WEXITSTATUS(wstatus);
    } else {
        return 1;
    }
    return 0;
}