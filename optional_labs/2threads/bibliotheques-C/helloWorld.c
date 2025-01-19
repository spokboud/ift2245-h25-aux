#include <stdio.h>
#include <unistd.h>

int main(){
    printf("Hello, World!\n");
    pause();
    return 0;
}
// strace -e trace=write ./Hello_World_c
