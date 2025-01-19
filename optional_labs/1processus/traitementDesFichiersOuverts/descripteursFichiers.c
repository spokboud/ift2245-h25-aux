#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<fcntl.h>
// https://www.tecmint.com/explanation-of-everything-is-a-file-and-types-of-files-in-linux/
void main(int argc, char **argv) {
    if(argc < 4){
        printf("Usage : ./descripteursFichiers IN OUT commande [argument...]\n");
        exit(EXIT_FAILURE);
    }
    int fd_in, fd_out, retour_dup_0;
    fd_in = open(argv[1], O_RDONLY);
    if(fd_in == -1){
        perror(argv[1]);
    }
    // echo $$ --> pid du terminal appelant
    // man creat.2
    // open(argv[2], O_WRONLY|O_CREAT|O_TRUNC|O_NONBLOCK|O_APPEND, 0644)
    // https://cppsecrets.com/users/41129711010797106994610011511264103109971051084699111109/Linux-Difference-between-tty-and-pts.php
    // https://unix.stackexchange.com/questions/60641/linux-difference-between-dev-console-dev-tty-and-dev-tty0
    // https://www.kernel.org/doc/Documentation/admin-guide/devices.txt
    // http://ftp.lyx.org/pub/sgml-tools/website/HOWTO/Text-Terminal-HOWTO/t1162.html
    // man mknod.2
    // man makedev.3

    // If pathname does not exist, create it as a regular file.
    fd_out = creat(argv[2], 0666);
    if(fd_out == -1){
        perror(argv[2]);
    }
    if(dup2(fd_in, 0) == -1 || dup2(fd_out, 1) == -1 || dup2(fd_out, 2) == -1){
        perror("echec dup2");
    }
    close(fd_in);
    close(fd_out);
    execvp (argv[3], argv+3);
    perror("La commande ne peut pas être exécutée");
    exit(EXIT_FAILURE);
}
/******************************************************************
 * ./descripteursFichiers /dev/null /dev/tty ls -l /proc/self/fd/ *
 *                                                                *
 * Affiche le contenu du fichier /proc/self/fd, qui représente:   *
 * 1) la table des descripteurs                                   *
 * 2) les fichiers sur lesquels ils pointent                      *
 * -------------------------------------------------------------- *
 *  0 -> /dev/null                                                *
 *  1 -> /dev/tty                                                 *
 *  2 -> /dev/tty                                                 *
 *  3 -> /proc/12375/fd                                           *
 ******************************************************************/















///////////////////////////////////////
/*************************
 * vim devices.txt       *
 * :colorscheme desert   *
 * :TOhtml               *
 * :wq                   *
 *************************/