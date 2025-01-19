#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[]){

	struct stat statbuf;
	int retour_stat, i, j;

	if(argc == 1){
		printf("Usage: %s fichier1 [fichier2 ...]\n", argv[0]);
		return 1;
	}

	for (i=1; i<argc; i++){

		struct passwd *pw = NULL;
		struct group *gr = NULL;
		char type[30];
		char permission[4]="";
		char droits[20]="";
		char Date_modification[30]="";

		retour_stat = lstat(argv[i], &statbuf);

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

		if(i == 1){
			printf("| Numéro inode | Type               | Droits      | Nombre de lien durs | Utilisateur | Groupe | Taille      | Dernière modification \n");
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
		printf("| %-12ld | %-20s | %-11s | %-19ld | %-11s | %-6s | %-11ld | %-29s \n", statbuf.st_ino, droits, type, statbuf.st_nlink, pw->pw_name, gr->gr_name, statbuf.st_size, Date_modification);

	}

	return 0;

}
