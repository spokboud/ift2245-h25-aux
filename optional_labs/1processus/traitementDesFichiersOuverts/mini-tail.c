#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
// fuser -vu /home/guib
// https://www.tecmint.com/10-lsof-command-examples-in-linux/
int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }
    int file_descriptor;
    off_t old_size;
    struct stat file_stat;
    file_descriptor = open(argv[1], O_RDONLY);
    if (file_descriptor == -1) {
        perror("Erreur lors du open");
        return 1;
    }
    if (fstat(file_descriptor, &file_stat) == -1) {
        perror("Erreur lors du stat");
        return 1;
    }
    old_size = file_stat.st_size;

    char* buffer = NULL;
    while (1) {
        sleep(1);
        if (fstat(file_descriptor, &file_stat) == -1) {
            perror("Erreur lors du stat");
            return 1;
        }
        if (file_stat.st_size > old_size) {
            int size = file_stat.st_size - old_size;
            buffer = malloc(size + 1);
            if (buffer == NULL) {
                perror("Erreur lors du malloc");
                return 1;
            }
            buffer[size] = '\0';
            lseek(file_descriptor, old_size, SEEK_SET);
            if (read(file_descriptor, buffer, size) == -1){
                perror("Erreur lors du read");
                return 1;
            }
            printf("%s\n", buffer);
        } else if (file_stat.st_size < old_size) {
            buffer = malloc(file_stat.st_size + 1);
            if (buffer == NULL) {
                perror("Erreur lors du malloc");
                return 1;
            }
            buffer[file_stat.st_size] = '\0';
            if (read(file_descriptor, buffer, file_stat.st_size) == -1){
                perror("Erreur lors du read");
                return 1;
            }
            printf("--- Le fichier a été tronqué! ---\n%s\n", buffer);
        }
        old_size = file_stat.st_size;
        free(buffer);
        buffer = NULL;
        lseek(file_descriptor, 0, SEEK_SET);
    }
}