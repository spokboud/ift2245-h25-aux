#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>

int main(int argc, char** argv){
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "sock", sizeof(addr.sun_path)-1);

    int res = bind(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (res == -1) {
        perror("bind");
        exit(1);
    }
//    listen(sock, SOMAXCONN);
    listen(sock, 0);

    char buf[100];
    int cli, resRead, resWrite, compteur=0;
    while(1){
        fprintf(stderr, "**En écoute..**\n");
        cli = accept(sock, NULL, NULL);
        if(cli == -1){
            perror("accept");
            exit(1);
        }
        fprintf(stderr, "**Client n°%d: Début session**\n",++compteur);
        while((resRead = read(cli, buf, sizeof(buf))) ){
            if(resRead == -1){
                perror("read cli");
                break;
            }
            resWrite = write(cli, buf, resRead);
            if(resWrite==-1){
                perror("write cli");
                break;
            }
        }
        close(cli);
        fprintf(stderr, "**Client n°%d: Fin session**\n",compteur);
    }
    close(sock);
    unlink("sock");
    return 0;
}
