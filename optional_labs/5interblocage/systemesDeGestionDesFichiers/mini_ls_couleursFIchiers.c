#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char * argv[]) {
        struct stat statbuf, sb;
        int retour_stat, i = 0, j = 0;
        char fichier_source[PATH_MAX * 2 +5] = ""; // 2 fois la longueur maximale + de l'espace pour la flèche
        char type[100];

        if (argc == 1) {
                printf("Usage: %s fichier1 [fichier2 ...]\n", argv[0]);
                return 1;
        }

        for (i = 1; i < argc; i++) {
                struct passwd * pw = NULL;
                struct group * gr = NULL;
                char permission[4] = "";
                char droits[20] = "";
                char Date_modification[30] = "";

                retour_stat = lstat(argv[i], & statbuf);
                if (retour_stat == -1) {
                        perror("Erreur de la récupération des informations");
                        return 1;
                }

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

                for (j = 0; j < 4; j++) {
                        switch (permission[j]) {
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

                pw = getpwuid(statbuf.st_uid);
                gr = getgrgid(statbuf.st_gid);

                strftime(Date_modification, 30, "%b %e %H:%M", localtime( & statbuf.st_mtime));

                if (i == 1) {
                        printf("| Numéro inode | Droits      | Type               | Nombre de lien durs | Utilisateur | Groupe | Taille      | Dernière modification | fichier                    \n");
                }
                printf("| %-12ld | %-11s | %-16s | %-19ld | %-11s | %-6s | %-11ld | %-21s |",
						statbuf.st_ino, droits, type, statbuf.st_nlink, pw -> pw_name, gr->gr_name, statbuf.st_size, Date_modification);
                if (strcmp(type, "répertoire") == 0) {
                        printf("\033[1;34m");
                        printf(" %s \n", argv[i]);
                        printf("\033[0m");
                } else if (strcmp(type, "fichier standard") == 0 && (statbuf.st_mode & 0111)) {
                        printf("\033[1;32m");
                        printf(" %s \n", argv[i]);
                        printf("\033[0m");
                } else if (strcmp(type, "lien symbolique") == 0) {
                        int rl = readlink(argv[i], fichier_source, statbuf.st_size + 1);
						if (rl == -1) {
							perror("Erreur en obtenant les infos sur le lien symbolique");
							return 1;
						}
                        if (stat(argv[i], & sb) == -1) {
                                printf("\033[1;31m");
                                printf(" %s ", argv[i]);
                                printf("\033[0m");
                                printf("->");
                                printf("\033[1;31m");
                                printf(" %s\n", fichier_source);
                                printf("\033[0m");
                        } else {
                                printf("\033[1;36m");
                                printf(" %s ", argv[i]);
                                printf("\033[0m");
                                printf("-> %s\n", fichier_source);
                        }
                } else {
                        printf(" %s \n", argv[i]);
                }

        }

        return 0;

}
