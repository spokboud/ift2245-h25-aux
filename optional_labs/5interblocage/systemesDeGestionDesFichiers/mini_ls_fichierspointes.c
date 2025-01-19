#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char* argv[]){

	struct stat statbuf;
	int retour_stat, i=0, j=0;
	char lien_sym[30]="";


	if(argc == 1){
		printf("Usage: %s fichier1 [fichier2 ...]\n", argv[0]);
		return 1;
	}

	for (i=1; i<argc; i++){

		struct passwd *pw = NULL;
		struct group *gr = NULL;
		char permission[4]="";
		char droits[20]="";
		char Date_modification[30]="";
		char nom_fichier[100];

		retour_stat = lstat(argv[i], &statbuf);
		if(retour_stat == -1){
			perror("Erreur de la récupération des informations");
			return 1;
		}
		strcpy(nom_fichier, argv[i]) ;

		if(readlink(argv[i], lien_sym, statbuf.st_size+1) != -1){
			strcat(nom_fichier," -> ");
			strcat(nom_fichier, lien_sym);
		}
			sprintf(permission, "%o", statbuf.st_mode & 0777);

			for(j=0; j<4; j++){
				switch (permission[j]){
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


			pw = getpwuid(statbuf.st_uid);
			gr = getgrgid(statbuf.st_gid);

			strftime(Date_modification, 30, "%b %e %H:%M", localtime(&statbuf.st_mtime));
			if(i == 1){
				printf("| Numéro inode | Droits      | Nombre de lien durs | Utilisateur | Groupe | Taille      | Dernière modification | fichier                    \n");
			}
			printf("| %-12ld | %-11s | %-19ld | %-11s | %-6s | %-11ld | %-21s | %s \n", statbuf.st_ino, droits, statbuf.st_nlink, pw->pw_name, gr->gr_name, statbuf.st_size, Date_modification, nom_fichier);


	}

	return 0;

}
