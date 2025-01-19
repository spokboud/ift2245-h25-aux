//
// Created by guib on 2021-07-20.
//

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
// man signal-safety
void gere(int sig) {
    for(int i=0; i<5; i++) {
        printf("Itération %d pour la gestion du signal %d\n", i+1, sig);
        if(i==2)
            sleep(5);//afin d'avoir le temps de placer un ctrl+c
    }
    exit(1);
}

int main(void) {
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask,SIGINT);//pour ignorer SIGINT durant l'execution de la gérante
    action.sa_flags = 0;
    action.sa_handler = gere;
    sigaction(SIGUSR1, &action, NULL);
    pause();
    return 0 ;
}