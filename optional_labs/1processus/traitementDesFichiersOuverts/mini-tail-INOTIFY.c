#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <fcntl.h>

struct file_watch {
    char* path;
    int fd;
    int wd;
    off_t size;
};

void handle_mod(int inotif, struct file_watch* watched, int num_wds) {
    struct inotify_event* event;
    char buf[sizeof(struct inotify_event)];
    int len = read(inotif, buf, sizeof(struct inotify_event));
    struct stat statbuf;
    char* content = NULL;
    char* msg = NULL;
    while (len > 0) {
        event = (struct inotify_event *)buf;
        for (int i = 0; i < num_wds; i++) {
            if (watched[i].wd == event->wd) {
                fstat(watched[i].fd, &statbuf);
// Si la taille a augmentée, on affiche le nouveau contenu
                if (statbuf.st_size > watched[i].size) {
                    msg = "";
                    int size = statbuf.st_size - watched[i].size;
                    content = malloc(size + 1);
                    content[size] = '\0';
                    read(watched[i].fd, content, size);
// Si la taille a baissée, on affiche tout le contenu
                } else if (statbuf.st_size < watched[i].size) {
                    msg = " file truncated";
                    lseek(watched[i].fd, 0, SEEK_SET);
                    content = malloc(statbuf.st_size + 1);
                    content[statbuf.st_size] = '\0';
                    read(watched[i].fd, content, statbuf.st_size);
                } else {
                    continue;
                }
                printf("File: %s%s\n%s\n", watched[i].path, msg, content);
                free(content);
                content = NULL;
                watched[i].size = statbuf.st_size;
            }
        }

        len = read(inotif, buf, sizeof(struct inotify_event));
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }
    // man inotify
    // man inotify_init1.2
    // man inotify_init2qq.2
    /*****************************
     *
     *****************************/
    int inotif = inotify_init();
    if (inotif == -1) {
        perror(NULL);
        return -1;
    }
    struct file_watch* watched = malloc(sizeof(struct file_watch) * argc-1);
    struct stat statbuf;
    char* content = NULL;
    // pour chaque fichier, on ajoute un watcher inotify, on remplit le struct et on affiche le contenu initial
    for (int i = 1; i < argc; i++) {
        watched[i-1].path = argv[i];
        watched[i-1].wd = inotify_add_watch(inotif, argv[i], IN_MODIFY);
        if (watched[i-1].wd == -1) {
            perror("Add watch");
            free(watched);
            return -1;
        }

        watched[i-1].fd = open(watched[i-1].path, O_RDONLY);
        if (watched[i-1].fd == -1) {
            perror("Open");
            free(watched);
            return -1;
        }
        int st = fstat(watched[i-1].fd, &statbuf);
        if (st == -1) {
            perror("Stat");
            free(watched);
            return -1;
        }
        watched[i-1].size = statbuf.st_size;
        content = realloc(content, watched[i-1].size + 1);
        if (content == NULL) {
            perror("Realloc");
            free(watched);
            return -1;
        }
        content[watched[i-1].size] = '\0';
        int rd = read(watched[i-1].fd, content, watched[i-1].size);
        if (rd == -1) {
            perror("Read");
            free(watched);
            return -1;
        }
        printf("File: %s\n%s\n", watched[i-1].path, content);
    }
    free(content);

    while (1) {
        handle_mod(inotif, watched, argc-1);
    }
    return 0;
}