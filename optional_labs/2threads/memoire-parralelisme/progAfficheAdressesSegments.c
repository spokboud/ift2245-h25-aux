/****************************************************
 * Programme qui permet d'afficher les adresses de: *
 * - fin de segments de texte;                      *
 * - des données initialisées;                      *
 * - des données non initialisées;                  *
 * - du tas.                                        *
 ****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
extern char etext, edata, end;
int main(int argv, char *argc[]){
    printf("   etext %p\n", &etext);
    printf("   edata %p\n", &edata);
    printf("   end   %p\n", &end);
    printf("   sbrk  %p\n", sbrk(0));
    pause();
    return 42;
}
