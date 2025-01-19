#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include <poll.h>

/******************************************************************************************************
 * Le programme cli.c ne permet pas une communication synchronisée, vue qu’il suit le schéma
 * suivant en boucle lecture->envoie au serveur->attente d’une réponse. /cli < /usr/share/dict/words
 * remplace le stdin par le fichier /usr/share/dict/words et commence à lire une ligne après l’autre
 * et l’envoyer au serveur, qui ensuite va l’envoyer aux autres clients. strings /dev/urandom | ./cli
 * crée un tube et on va lire une ligne après l’autre à partir du tube, et à chaque fois on va envoyer
 * au serveur qui va ensuite envoyer aux autres clients.
 * ****************************************************************************************************/

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

    char buf[100], buf_copie[120];
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
                if((fds[i].revents & POLLHUP) !=0) {
                    fprintf(stderr, "%d **Un client s'est deconnecté**\n", ++ligne_log);
                    close(fds[i].fd);
                    fds[i].fd = -1;//pour que poll ignore cette case
                    nbr_clients--;
                }else if(fds[i].revents == POLLIN){
                    fprintf(stderr, "%d **Le serveur répond**\n", ++ligne_log);
                    resRead = read(fds[i].fd, buf, sizeof(buf)-1);
                    if(resRead == -1){
                        perror("read");
                        return 1;
                    }
                    for(int j=1; j<nfds ;j++){
                        if(j!=i && fds[j].fd!=-1){
                            buf[resRead] = '\0';
                            snprintf(buf_copie, sizeof(buf_copie)-1, "Client %d: %s", i+1,buf);
                            resWrite = write(fds[j].fd, buf_copie, resRead+10);
                            if(resWrite == -1){
                                perror("write");
                                exit(1);
                            }
                        }
                    }
                }
            }
        }
    }
    close(sock);
    unlink("sock");
    return 0;
}
// cd 3173/labos/11_sockets-multiplexage/