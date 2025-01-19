#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<poll.h>

// multiplexage

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
    //listen(sock, 10);
    listen(sock, 0);

    //PRÉPARATION DU POLL
    struct pollfd *fds=NULL;
    nfds_t nfds=1;
    int resPoll;
    fds = malloc(sizeof(struct pollfd));
    if(fds==NULL){
        perror("malloc fds");
        exit(1);
    }
    fds[0].fd = sock;
    fds[0].events = POLLIN;

    char buf[100];
    int resRead, resWrite, ligne_log=0, nbr_clients=0, nbre_client_max=10, cli;
    while(1){
        fprintf(stderr, "%d **En écoute..**\n", ++ligne_log);
        resPoll = poll(fds, nfds, -1);
        if(resPoll==-1){
            perror("poll");
            exit(1);
        }
        for(int i=0; i<nfds ;i++){
            if(i==0 && fds[i].revents != 0 && fds[i].revents == POLLIN ){
                if(nbr_clients<nbre_client_max){
                    fprintf(stderr, "%d **Un nouveau client vient de se connecter**\n", ++ligne_log);
                    nfds++;
                    fds = realloc(fds, sizeof(struct pollfd)*nfds);
                    if(fds==NULL){
                        perror("realloc fds");
                        exit(1);
                    }
                    fds[nfds-1].fd = accept(sock, NULL, NULL);
                    if(fds[nfds-1].fd == -1){
                        perror("accept");
                        exit(1);
                    }
                    fds[nfds-1].events = POLLIN | POLLHUP;
                    nbr_clients++;
                }else{
                    cli = accept(sock, NULL, NULL);
                    if(cli == -1){
                        perror("accept");
                        exit(1);
                    }
                    close(cli);
                    fprintf(stderr, "%d **Un client vient d'être refusé(max clients=%d)**\n", ++ligne_log, nbre_client_max);
                }
            }else if(fds[i].revents != 0){
                if((fds[i].revents & POLLHUP) != 0 ){
                    fprintf(stderr, "%d **Un client s'est deconnecté**\n", ++ligne_log);
                    close(fds[i].fd);
                    fds[i].fd = -1;//pour que poll ignore cette case
                    nbr_clients--;
                }else if(fds[i].revents == POLLIN){
                    fprintf(stderr, "%d **Le serveur répond**\n", ++ligne_log);
                    resRead = read(fds[i].fd, buf, sizeof(buf));
                    if(resRead == -1){
                        perror("read");
                        return 1;
                    }
                    resWrite = write(fds[i].fd, buf, resRead);
                    if(resWrite == -1){
                        perror("write");
                        exit(1);
                    }
                }
            }
        }
    }
    close(sock);
    unlink("sock");
    return 0;
}