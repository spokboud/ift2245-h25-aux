#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<signal.h>
// https://squidarth.com/networking/systems/rc/2018/05/28/using-raw-sockets.html
void gere(int sig) {
    fprintf(stderr, "Connexion perdue\n");
    exit(1);
}

int main(int argc, char **argv)
{
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = gere;
    sigaction(SIGPIPE, &action, NULL);



    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "sock", sizeof(addr.sun_path)-1);

    fprintf(stderr, "**Connexion en cours**\n");
    int res = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if(res==-1) {
        perror("connect");
        exit(1);
    }
    fprintf(stderr, "**Connecté**\n");
    int resWrite, resRead;

    char buf[100];
    while(fgets(buf, sizeof(buf)-1, stdin)!=NULL){
        fprintf(stderr, "**Envoi en cours**\n");
        resWrite=write(sock, buf, strlen(buf));
        if(resWrite==-1){
            perror("write cli");
            exit(1);
        }
        fprintf(stderr, "**Message envoyé**\n");
        fprintf(stderr, "**En attente du serveur...**\n");
        resRead = read(sock, buf, sizeof(buf)-1 );
        if(resRead == -1){
            perror("read cli");
            return 1;
        }
        buf[resRead]='\0';
        printf("Serveur: %s",buf);
    }

    close(sock);
    fprintf(stderr, "**Connexion teminée**\n");
    return 0;
}

