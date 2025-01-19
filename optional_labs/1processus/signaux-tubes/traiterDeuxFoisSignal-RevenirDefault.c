//
// Created by guib on 2021-07-20.
//
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void gere(int sig) {
    printf("Réception du signal SIGINT\n");
}

int main(void) {
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = gere;
    sigaction(SIGINT, &action, NULL);
    pause();
    pause();
    action.sa_handler = SIG_DFL;
    sigaction(SIGINT, &action, NULL);
    pause();
    return 0 ;
}