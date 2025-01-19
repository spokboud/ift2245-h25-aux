#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


const char buf[1024*1024*4] = "Hello";
extern char etext, edata, end;
int main(int argv, char *argc[]){
    printf("   etext %p\n", &etext);
    printf("   edata %p\n", &edata);
    printf("   end   %p\n", &end);
    // Calling sbrk() with an increment of 0 can be used to find the current location of the program break.
    printf("   sbrk  %p\n", sbrk(0));
    pause();
    return 42;
}
