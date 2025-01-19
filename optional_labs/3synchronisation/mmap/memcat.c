// Le programme memcat qui prend en argument le fichier à projeter et en affiche le contenu.

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/mman.h>

int main(int argc, char **argv){
    if(argc != 2){
        printf("usage : %s nom_fichier\n", argv[0]);
        return 1;
    }

    int fd, resWrite;
    struct stat stat;
    char *buf;

    fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        perror("open");
        return 1;
    }
    if(fstat(fd, &stat) == -1){
        perror("stat");
        return 1;
    }
    buf = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(buf == (char *)MAP_FAILED){
        perror("mmap");
        return 1;
    }
    close(fd);

    resWrite=write(1, buf, stat.st_size);
    if(resWrite == -1){
        perror("write");
        return 1;
    }
    munmap(buf, stat.st_size);

    return 0;

}