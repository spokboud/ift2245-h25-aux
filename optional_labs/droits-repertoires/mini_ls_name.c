#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/sysmacros.h>

int main(int argc, char* argv[]){
    struct stat statbuf;
    int retour_stat, i;
    struct passwd *pw;
    struct group *gr;
    char type[30];
    char permission[4];
    char droits[20]="";
    char Date_modification[30];
    int arg = 1, show_name = 0;

    if(argc < 2){
        printf("Usage: %s nom_du_fichier\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "-n") == 0) {
        arg = 2;
        show_name = 1;
    }

    retour_stat = lstat(argv[arg], &statbuf);

    if(retour_stat == -1){
        perror("Erreur de la récupération des informations");
        return 1;
    }
    switch (statbuf.st_mode & S_IFMT) {
        case S_IFDIR:  strcpy(type, "répertoire");          break;
        case S_IFLNK:  strcpy(type, "lien symbolique");     break;
        case S_IFREG:  strcpy(type, "fichier standard");    break;
        default:       strcpy(type, "Autre");               break;
    }

    sprintf(permission, "%o", statbuf.st_mode & 0777);

    for(i=0; i<4; i++){
        switch (permission[i]){
            case '0':
                strcat(droits,"---");
                break;
            case '1':
                strcat(droits,"--x");
                break;
            case '2':
                strcat(droits,"-w-");
                break;
            case '3':
                strcat(droits,"-wx");
                break;
            case '4':
                strcat(droits,"r--");
                break;
            case '5':
                strcat(droits,"r-x");
                break;
            case '6':
                strcat(droits,"rw-");
                break;
            case '7':
                strcat(droits,"rwx");
                break;

        }
    }

    strftime(Date_modification, 30, "%b %e %H:%M", localtime(&statbuf.st_mtime));
    printf("| Numéro inode | Type                 | Droits      | Nombre de lien durs | Utilisateur | Groupe | Taille      | Dernière modification \n");
    if (show_name) {
        pw = getpwuid(statbuf.st_uid);
        gr = getgrgid(statbuf.st_gid);
        printf("| %-12ld | %-20s | %-11s | %-19ld | %-11s | %-6s | %-11ld | %-29s \n", statbuf.st_ino, type, droits, statbuf.st_nlink, pw->pw_name, gr->gr_name, statbuf.st_size, Date_modification);
    } else {
        printf("| %-12ld | %-20s | %-11s | %-19ld | %-11d | %-6d | %-11ld | %-29s \n", statbuf.st_ino, type, droits, statbuf.st_nlink, statbuf.st_uid, statbuf.st_gid, statbuf.st_size, Date_modification);
    }


    return 0;

}

