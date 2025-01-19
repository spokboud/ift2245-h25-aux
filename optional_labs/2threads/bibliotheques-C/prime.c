/******************************************************************************************************************************************************
 * Ça permet de mentionner à l’éditeur le lien que nous allons faire appel à la bibliothèque maths. Pour plus d’information man 3 sqrt
 * ldd fichier permet de voir où sont les bibliothèques d’un exécutable. Les bibliothèques avec l’extension .a sont des bibliothèques statiques,
 * et les bibliothèques avec l’extension .so sont des bibliothèques dynamiques. ldd ./prime. La fonction sqrt() n’est pas un appel système, car
 * elle ne nécessite pas le mode noyau pour s’exécuter. Pour approcher la racine carrée d’un nombre, il y a plusieurs méthodes : méthode de Newton,
 * méthode de dichotomie, …etc, et souvent sqrt est fourni par les processeurs (avec leurs propres techniques d’approximation). Remarque. Les appels
 * système sont liés à un système d’exploitation précis. Il y a plusieurs systèmes d’exploitation et ils viennent en plusieurs versions. Les standards
 * comme POSIX ne sont pas toujours implémentés et peuvent aussi être étendus. De manière générale, pour savoir si une fonction est un appel système,
 * faut intéroger l’OS en question. Sous Linux man syscalls va lister tous les appels système supportés.
 * ****************************************************************************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Vous devez fournir la borne supérieure\n");
        return -1;
    }
    char* endptr;
    long int argument = strtol(argv[1], &endptr, 0); //strtol pour convertir de String vers Integer
    if (*argv[1] == '\0' || *endptr != '\0' || argument <= 0) {
        fprintf(stderr, "La borne fournie doit être un entier supérieur à 0\n");
        return -1;
    }
    long int racine_i, j, i ;
    size_t cap = 8;
    long int* primes = malloc(cap * sizeof(long int)); // Tableau de nombres premiers trouvés (à allouer dans le tas et réallouer)
    if (primes == NULL) {
        perror("Allocation");
        return 1;
    }
    size_t nb = 0 , k;
    bool prime ;
    for (i=2; i<=argument; i++) {
        prime = true ;
        racine_i = (long int)sqrt((double)i) ;
        for (k=0; k<nb; k++) {
            j = primes[k] ;
            if ( j > racine_i ) {
                break ; // on trouvera plus de diviseur, inutile de continuer de chercher
            }
            if (i%j == 0) {
                prime = false ; // c'est divisible, donc pas premier
                break ;
            }
        }
        if (prime) {
            // on enregistre et on passe au nombre suivant
            primes[nb] = i ;
            nb++ ;
            if(nb > cap){
                cap *= 2;
                primes = realloc( primes, sizeof(long int) * cap);
                if(primes == NULL){
                    fprintf(stderr, "La valeur de errno: %d\nInterprétation: %s",errno, strerror(errno)) ;
                    return 1;
                }
            }
        }
    }
    printf("Nombre de nombres premiers trouvés : %ld\n", nb) ;
    free(primes);
    //Affichage des nombres premiers trouvés
    /*printf("Affichage des nombres premiers trouvés : \n") ;
    for(int long h = 0 ; h!=nb ; h++){
        printf("%ld  ", primes[h] ) ;
    }
    printf("\n") ;*/
    return 0 ;

}

// ./helloWorld_c | cowsay &
// ls -la /proc/`pgrep hello`/fd
// strace ./helloWorld_cpp 2>&1 | grep write
