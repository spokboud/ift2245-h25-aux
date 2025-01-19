// Le programme memcount qui affiche le nombre de fois où un certain motif fourni
// en argument est présent dans le fichier.

#define _GNU_SOURCE
#include <string.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/mman.h>


int main(int argc, char **argv){
    if(argc != 3){
        printf("usage : %s nom_fichier motif\n", argv[0]);
        return 1;
    }

    int fd, compteur=0, taille;
    struct stat stat;
    char *buf=NULL, *adresse_occurence=NULL, *pointeur_buf=NULL;

    fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        perror("open");
        return 1;
    }
    if(fstat(fd, &stat) == -1){
        perror("stat");
        return 1;
    }
    buf = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(buf == (char *)MAP_FAILED){
        perror("mmap");
        return 1;
    }
    close(fd);

    taille = stat.st_size;
    pointeur_buf = buf ;

    while((adresse_occurence = memmem(pointeur_buf, taille, argv[2], strlen(argv[2]))) ){
        pointeur_buf = adresse_occurence+strlen(argv[2]);
        taille = stat.st_size - (pointeur_buf - buf);
        compteur++;
    }

    printf("Total : %d occurences\n", compteur);

    munmap(buf, stat.st_size);

    return 0;
}