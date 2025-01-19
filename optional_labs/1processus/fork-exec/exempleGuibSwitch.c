#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {
    pid_t pid; int wStatus;
    printf("Je m'identifie le pere, pid: %d\n", getpid());
    switch (pid = fork()) {
        case -1: // error code
            perror("fork error: ");
            return EXIT_FAILURE;
        case 0: // child code
            printf("Je suis l'enfant, pid: %d, pere: %d\n", getpid(), getppid());
            printf("variable pid: %d\n", pid);
            // exit() ou exec()
            exit(0);
        default: // parent code
            printf("Je suis le pere, pid: %d, mais\n", getpid());
            printf("variable pid: %d\n", pid);

    }
    // still parent code
    printf("Je suis toujours le pere, pid: %d, mais\n", getpid());
    printf("variable pid: %d\n", pid);
    wait(&wStatus);
    return 0;
}

