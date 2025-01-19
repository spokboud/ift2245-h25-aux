#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>


int main(int argc, char * argv[]) {
    struct stat statbuf;
    int retour_stat, i;
    char type[30];
    char permission[4];
    char droits[20] = "";
    char Date_modification[30];

    if (argc != 2) {
        printf("Usage: %s nom_du_fichier\n", argv[0]);
        return 1;
    }

    retour_stat = stat(argv[1], & statbuf);

    if (retour_stat == -1) {
        perror("Erreur de la récupération des informations");
        return 1;
    }
    /********************************************************************************************
    *    S_IFMT is a bit mask for file type (see man stat)
    *    bitwise AND-ing directly with mystat.st_mode (mystat.st_mode & S_IFMT)
    *    means to consider only the bits involved to determine the file type
    *    (regular file, socket, block or char device, etc.)
    *
    *    doing a bitwise AND of mystat.st_mode with the bitwise negated bitmask
    *    (mystat.st_mode & ~S_IFMT) means to ignore the bits explained above,
    *    keeping just the ones need to determine file permission (the 9 lines below that command).
    *********************************************************************************************/

    switch (statbuf.st_mode & S_IFMT) {
    case S_IFDIR:
        strcpy(type, "répertoire");
        break;
    case S_IFLNK:
        strcpy(type, "lien symbolique");
        break;
    case S_IFREG:
        strcpy(type, "fichier standard");
        break;
    default:
        strcpy(type, "Autre");
        break;
    }

    sprintf(permission, "%o", statbuf.st_mode & 0777);
    for (i = 0; i < 4; i++) {
        switch (permission[i]) {
        case '0':
            strcat(droits, "---");
            break;
        case '1':
            strcat(droits, "--x");
            break;
        case '2':
            strcat(droits, "-w-");
            break;
        case '3':
            strcat(droits, "-wx");
            break;
        case '4':
            strcat(droits, "r--");
            break;
        case '5':
            strcat(droits, "r-x");
            break;
        case '6':
            strcat(droits, "rw-");
            break;
        case '7':
            strcat(droits, "rwx");
            break;
        }
    }

    unsigned int sticky = statbuf.st_mode & 07000;
    if (sticky == 04000)
        droits[2] = 's';
    else if (sticky == 02000)
        droits[5] = 's';
    else if (sticky == 01000)
        droits[8] = 't';

    strftime(Date_modification, 30, "%b %e %H:%M", localtime( & statbuf.st_mtime));
//    retour_stat = lstat(argv[1], & statbuf);
    printf("| Numéro inode | Type                 | Droits      | Nombre de lien durs | uid      | gid      | Taille      | Dernière modification \n");
    printf("| %-12ld | %-20s | %-11s | %-19ld | %-8d | %-8d | %-11ld | %-29s \n", statbuf.st_ino, type, droits, statbuf.st_nlink, statbuf.st_uid, statbuf.st_gid, statbuf.st_size, Date_modification);

    return 0;

}
