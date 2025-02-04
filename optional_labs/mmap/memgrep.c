// Le programme memgrep qui affiche,
// dans l’ordre inverse, toutes les
// lignes où le motif recherché est présent.

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

    int fd, taille;
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
    char* adresse_debut, *adresse_fin;

    // Pour chaque instance de la chaine, on trouve le début et la fin de la ligne pour l'afficher
    while((adresse_occurence = memmem(pointeur_buf, taille, argv[2], strlen(argv[2]))) ){

        adresse_debut = memrchr(buf, '\n', adresse_occurence-buf);
        if (adresse_debut == NULL) { //si c'est la première ligne
            adresse_debut = buf;
        } else {
            adresse_debut++;//après le retour de ligne
        }

        adresse_fin = memchr(adresse_occurence, '\n', taille);
        if (adresse_fin == NULL) { //si c'est la dernière ligne du fichier
            adresse_fin = buf + taille;
        }
        write(1, adresse_debut, adresse_fin-adresse_debut+1);

        // on continue la recherche à partir de la ligne suivante pour éviter d'afficher plusieurs fois une ligne qui contient plus qu'une instance de la chaine recherchée
        pointeur_buf = adresse_fin+1;
        taille = stat.st_size - (pointeur_buf - buf);
    }

    munmap(buf, stat.st_size);

    return 0;
}