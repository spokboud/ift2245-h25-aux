# Mini ls (suite)
### -n pour afficher le nom du propriétaire et du groupe
```c
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
```
### Affichez un + si le fichier a des ACL
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <time.h>
#include <unistd.h>

void mode_to_chars(mode_t mode, char* chars) {
if (S_ISUID & mode)
chars[0] = 's';
else if (S_ISGID & mode)
chars[0] = 'g';
else if (S_ISVTX & mode)
chars[0] = 'd';
else
chars[0] = '-';
chars[1] = S_IRUSR & mode ? 'r' : '-';
chars[2] = S_IWUSR & mode ? 'w' : '-';
chars[3] = S_IXUSR & mode ? 'x' : '-';
chars[4] = S_IRGRP & mode ? 'r' : '-';
chars[5] = S_IWGRP & mode ? 'w' : '-';
chars[6] = S_IXGRP & mode ? 'x' : '-';
chars[7] = S_IROTH & mode ? 'r' : '-';
chars[8] = S_IWOTH & mode ? 'w' : '-';
chars[9] = S_IXOTH & mode ? 'x' : '-';
chars[10] = 0;
}

int main(int argc, char* argv[]) {
int begin = 1;
if (2 > argc) {
return 1;
}

	struct stat file_stat;
	struct stat file_stat2;
	char mod_time[16];
	int st = 0;
	printf("Numéro inode  | Droits      | Nombre de liens  |    uid |    gid |    Taille | Dernière modification | \n");
	char perms[11];
	int color;
	for (int i = begin; i < argc; i++) {
		char* filename = argv[i];
		char* has_acl = "";
		st = lstat(filename, &file_stat);
		if (lgetxattr(filename, "system.posix_acl_access", NULL, 0) != -1)
			has_acl = "+";
		if (st != 0) {
			perror(NULL);
		} else {
			mode_to_chars(file_stat.st_mode, perms);
			strftime(mod_time, 15, "%b %d %H:%M", localtime(&file_stat.st_mtim.tv_sec));
			if (S_ISLNK(file_stat.st_mode)) {
				color = 96;
				char* pointed = malloc(file_stat.st_size + 1);
				pointed[file_stat.st_size] = 0;
				if (stat(filename, &file_stat2) == -1) {
					perror(NULL);
					color = 31;
				}
				readlink(filename, pointed, file_stat.st_size);
				printf("%-13lu | %-7s%-1s | %-16lu | %-6u | %-6u | %-9lu | %-21s | \e[%dm %s -> %s \e[0m\n", file_stat.st_ino, perms, has_acl, file_stat.st_nlink, file_stat.st_uid, file_stat.st_gid, file_stat.st_size, mod_time, color, filename, pointed);
				free(pointed);
			} else {
				if (S_ISDIR(file_stat.st_mode)) {
					color = 34;
				} else if (S_IXUSR & file_stat.st_mode) {
					color = 32;
				}
				printf("%-13lu | %-7s%-1s | %-16lu | %-6u | %-6u | %-9lu | %-21s | \e[%dm %s \e[0m\n", file_stat.st_ino, perms, has_acl, file_stat.st_nlink, file_stat.st_uid, file_stat.st_gid, file_stat.st_size, mod_time, color, filename);
			}
		}
	}


	return 0;
}
```
### Le programme mini_ls avec opendir readdir closedir
```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

void mode_to_chars(mode_t mode, char* chars) {
if (S_ISUID & mode)
chars[0] = 's';
else if (S_ISGID & mode)
chars[0] = 'g';
else if (S_ISVTX & mode)
chars[0] = 'd';
else
chars[0] = '-';
chars[1] = S_IRUSR & mode ? 'r' : '-';
chars[2] = S_IWUSR & mode ? 'w' : '-';
chars[3] = S_IXUSR & mode ? 'x' : '-';
chars[4] = S_IRGRP & mode ? 'r' : '-';
chars[5] = S_IWGRP & mode ? 'w' : '-';
chars[6] = S_IXGRP & mode ? 'x' : '-';
chars[7] = S_IROTH & mode ? 'r' : '-';
chars[8] = S_IWOTH & mode ? 'w' : '-';
chars[9] = S_IXOTH & mode ? 'x' : '-';
chars[10] = 0;
}

bool has_acl(char* filename) {
return lgetxattr(filename, "system.posix_acl_access", NULL, 0) != -1;
}

int list_file(char* filename, bool show_name) {
char perms[11];
int color;
struct stat file_stat;
struct stat file_stat2;
char* acl_prop = "";
char mod_time[16];
int st = lstat(filename, &file_stat);
if (st != 0) {
perror(NULL);
return -1;
}
if (has_acl(filename)) {
acl_prop = "+";
}
mode_to_chars(file_stat.st_mode, perms);
strftime(mod_time, 15, "%b %d %H:%M", localtime(&file_stat.st_mtim.tv_sec));
if (S_ISLNK(file_stat.st_mode)) {
color = 96;
char* pointed = malloc(file_stat.st_size + 1);
pointed[file_stat.st_size] = 0;
if (stat(filename, &file_stat2) == -1) {
perror(NULL);
color = 31;
}
readlink(filename, pointed, file_stat.st_size);
printf("%-13lu | %-7s%-1s | %-16lu | %-6u | %-6u | %-9lu | %-21s | \e[%dm %s -> %s \e[0m\n", file_stat.st_ino, perms, acl_prop, file_stat.st_nlink, file_stat.st_uid, file_stat.st_gid, file_stat.st_size, mod_time, color, filename, pointed);
free(pointed);
} else {
if (S_ISDIR(file_stat.st_mode)) {
color = 34;
} else if (S_IXUSR & file_stat.st_mode) {
color = 32;
}
printf("%-13lu | %-7s%-1s | %-16lu | %-6u | %-6u | %-9lu | %-21s | \e[%dm %s \e[0m\n", file_stat.st_ino, perms, acl_prop, file_stat.st_nlink, file_stat.st_uid, file_stat.st_gid, file_stat.st_size, mod_time, color, filename);
}
return 0;
}

int main(int argc, char* argv[]) {
int show_name = 0;
int begin = 1;
if (2 > argc) {
return 1;
}

	if (strcmp(argv[1], "-n") == 0) {
		show_name = 1;
		begin += 1;
	}
	int st = 0;
	printf("Numéro inode  | Droits      | Nombre de liens  |    uid |    gid |    Taille | Dernière modification | \n");
	struct stat file_stat;
	for (int i = begin; i < argc; i++) {
		char* filename = argv[i];
		st = lstat(filename, &file_stat);
		if (st != 0) {
			perror(NULL);
		} else if (S_ISDIR(file_stat.st_mode)) {
			DIR* dir = opendir(filename);
			if (dir == NULL) {
				perror(filename);
			} else {
				struct dirent* entry;
				do {
					errno = 0;
					entry = readdir(dir);
					if (entry != NULL) {
						char* fullname = malloc(strlen(filename) + strlen(entry->d_name) + 2);
						strcpy(fullname, filename);
						strcat(fullname, "/");
						strcat(fullname, entry->d_name);
						list_file(fullname, show_name);
					} else if (errno != 0) {
						perror("Reading directory");
					}
				} while (entry != NULL && errno != 0);
				closedir(dir);
			}
		} else {
			list_file(filename, show_name);
		}
	}
	return 0;
}
```
### OU
```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

void mode_to_chars(mode_t mode, char* chars) {
if (S_ISUID & mode)
chars[0] = 's';
else if (S_ISGID & mode)
chars[0] = 'g';
else if (S_ISVTX & mode)
chars[0] = 'd';
else
chars[0] = '-';
chars[1] = S_IRUSR & mode ? 'r' : '-';
chars[2] = S_IWUSR & mode ? 'w' : '-';
chars[3] = S_IXUSR & mode ? 'x' : '-';
chars[4] = S_IRGRP & mode ? 'r' : '-';
chars[5] = S_IWGRP & mode ? 'w' : '-';
chars[6] = S_IXGRP & mode ? 'x' : '-';
chars[7] = S_IROTH & mode ? 'r' : '-';
chars[8] = S_IWOTH & mode ? 'w' : '-';
chars[9] = S_IXOTH & mode ? 'x' : '-';
chars[10] = 0;
}

bool has_acl(char* filename) {
return lgetxattr(filename, "system.posix_acl_access", NULL, 0) != -1;
}

int list_file(char* filename, bool show_name) {
char perms[11];
int color;
struct stat file_stat;
struct stat file_stat2;
char* acl_prop = "";
char mod_time[16];
int st = lstat(filename, &file_stat);
if (st != 0) {
perror(NULL);
return -1;
}
if (has_acl(filename)) {
acl_prop = "+";
}
mode_to_chars(file_stat.st_mode, perms);
strftime(mod_time, 15, "%b %d %H:%M", localtime(&file_stat.st_mtim.tv_sec));
if (S_ISLNK(file_stat.st_mode)) {
color = 96;
char* pointed = malloc(file_stat.st_size + 1);
pointed[file_stat.st_size] = 0;
if (stat(filename, &file_stat2) == -1) {
perror(NULL);
color = 31;
}
readlink(filename, pointed, file_stat.st_size);
printf("%-13lu | %-7s%-1s | %-16lu | %-6u | %-6u | %-9lu | %-21s | \e[%dm %s -> %s \e[0m\n", file_stat.st_ino, perms, acl_prop, file_stat.st_nlink, file_stat.st_uid, file_stat.st_gid, file_stat.st_size, mod_time, color, filename, pointed);
free(pointed);
} else {
if (S_ISDIR(file_stat.st_mode)) {
color = 34;
} else if (S_IXUSR & file_stat.st_mode) {
color = 32;
}
printf("%-13lu | %-7s%-1s | %-16lu | %-6u | %-6u | %-9lu | %-21s | \e[%dm %s \e[0m\n", file_stat.st_ino, perms, acl_prop, file_stat.st_nlink, file_stat.st_uid, file_stat.st_gid, file_stat.st_size, mod_time, color, filename);
}
return 0;
}

int main(int argc, char* argv[]) {
int show_name = 0;
int begin = 1;
if (2 > argc) {
return 1;
}

	if (strcmp(argv[1], "-n") == 0) {
		show_name = 1;
		begin += 1;
	}
	int st = 0;
	printf("Numéro inode  | Droits      | Nombre de liens  |    uid |    gid |    Taille | Dernière modification | \n");
	struct stat file_stat;
	for (int i = begin; i < argc; i++) {
		char* filename = argv[i];
		st = lstat(filename, &file_stat);
		if (st != 0) {
			perror(NULL);
		} else if (S_ISDIR(file_stat.st_mode)) {
			DIR* dir = opendir(filename);
			if (dir == NULL) {
				perror(filename);
			} else {
				struct dirent* entry;
				do {
					errno = 0;
					entry = readdir(dir);
					if (entry != NULL) {
						char* fullname = malloc(strlen(filename) + strlen(entry->d_name) + 2);
						strcpy(fullname, filename);
						strcat(fullname, "/");
						strcat(fullname, entry->d_name);
						list_file(fullname, show_name);
					} else if (errno != 0) {
						perror("Reading directory");
					}
				} while (entry != NULL && errno != 0);
				closedir(dir);
			}
		} else {
			list_file(filename, show_name);
		}
	}
	return 0;
}
```
### Répertoire absolu
```c
#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
char absolute_path[PATH_MAX] = "";
char buffer[PATH_MAX];
struct stat file_stat;
struct dirent* entry;
int parent_dir_fd;
DIR* dir = NULL, *olddir;
stat("/", &file_stat);
ino_t root_inode = file_stat.st_ino;

	stat(".", &file_stat);
	ino_t cur_inode = file_stat.st_ino;
	int cur_dir_fd = open(".", 0);
	do {
		parent_dir_fd = openat(cur_dir_fd, "..", 0);
		olddir = dir;
		dir = fdopendir(parent_dir_fd);
		if (olddir != NULL) {
			//closedir ferme aussi le fd du dossier. On l'utilise aussi pour openat alors c'est désagréable pour nous. D'où ce if.
			closedir(olddir);
		}
		do {
			errno = 0;
			entry = readdir(dir);
			if (entry == NULL) {
				perror("Error finding in parent directory");
				return 1;
			}
		} while (entry->d_ino != cur_inode);
		strcpy(buffer, "/");
		strcat(buffer, entry->d_name);
		strcat(buffer, absolute_path);
		strcpy(absolute_path, buffer);

		cur_dir_fd = parent_dir_fd;
		fstat(cur_dir_fd, &file_stat);
		cur_inode = file_stat.st_ino;
	} while (cur_inode != root_inode);
	closedir(dir);
	printf("%s\n", absolute_path);

	return 0;
}
```