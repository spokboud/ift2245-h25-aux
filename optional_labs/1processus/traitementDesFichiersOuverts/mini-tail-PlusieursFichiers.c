#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }
    int fds[argc - 1];
    off_t prev_sizes[argc - 1];
    struct stat statbuf;
    int st;
    for (int i = 1; i < argc; i++) {
        fds[i-1] = open(argv[i], O_RDONLY);
        if (fds[i-1] == -1) {
            perror("Erreur lors de l'ouverture");
            return 1;
        }
        st = fstat(fds[i-1], &statbuf);
        if (st == -1) {
            perror("Erreur lors du stat");
            return 1;
        }
        prev_sizes[i-1] = statbuf.st_size;
    }
    char* buf = NULL;
    while (1) {
        sleep(1);
        for (int i = 0; i < argc-1; i++) {
            st = fstat(fds[i], &statbuf);
            if (st == -1) {
                perror("Erreur lors du stat");
                return 1;
            }
            if (statbuf.st_size > prev_sizes[i]) {
                int size = statbuf.st_size - prev_sizes[i];
                buf = malloc(size + 1);
                if (buf == NULL) {
                    perror("Erreur lors de l'allocation");
                    return 1;
                }
                buf[size] = '\0';
                lseek(fds[i], prev_sizes[i], SEEK_SET);
                int rd = read(fds[i], buf, size);
                printf("%s:\n%s\n", argv[i+1], buf);
            } else if (statbuf.st_size < prev_sizes[i]) {
                buf = malloc(statbuf.st_size + 1);
                if (buf == NULL) {
                    perror("Erreur lors de l'allocation");
                    return 1;
                }
                buf[statbuf.st_size] = '\0';
                int rd = read(fds[i], buf, statbuf.st_size);
                printf("%s:---Fichier a été tronqué---\n%s\n", argv[i+1], buf);
            }
            prev_sizes[i] = statbuf.st_size;
            free(buf);
            buf = NULL;
            lseek(fds[i], 0, SEEK_SET);
        }
    }

}
