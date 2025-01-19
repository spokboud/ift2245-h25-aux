#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void gere(int sig) {
    printf("Réception du signal SIGINT\n");
    exit(1);
}

int main(void) {
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = gere;
    sigaction(SIGINT, &action, NULL);
    pause();
    return 0 ;
}
