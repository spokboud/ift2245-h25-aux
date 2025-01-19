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
        // man openat
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
