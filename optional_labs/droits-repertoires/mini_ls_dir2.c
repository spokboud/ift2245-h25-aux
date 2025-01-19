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
