#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<pthread.h>

//variables globales
bool *work_list = NULL;
long int nb_thread ;
long int maximum ;

// Chaque thread travaille sur une fraction du tableau
// Chacun commence à un indice différent puis "saute" par-dessus les autres
void *do_work(void *ptr)
{
    long int depart = (long int)ptr;
    bool is_prime= false;

    for(long int i = depart; i <= maximum; i+= nb_thread)
    {
        is_prime = true;
        // C'est inefficace, car on parcourt tous les entiers
        // au lieu de tester seulement les nombres premiers
        // mais c'est plus simple à coder (pas de synchronisation nécessaire)
        for(long int j=2;j <= (long int )sqrt((double)i); j++)
        {
            if(i%j == 0){
                is_prime = false;
                break;
            }
        }

        if(is_prime){
            work_list[i] = true;
        }
    }
}

int main(int argc, char **argv)
{
    int nb=0, i, depart_argument;

    if (argc < 3) {
        fprintf(stderr, "Vous devez fournir la borne supérieure et le nombre de threads\n");
        return 1;
    }
    char* endptr = NULL;
    maximum = strtol(argv[1], &endptr, 0); // Entier maximum à tester
    if (*endptr != '\0' || maximum <= 0) {
        fprintf(stderr, "La borne supérieure doit être un nombre entier supérieur à 0\n");
        return 1;
    }
    endptr = NULL;
    nb_thread = strtol(argv[2], &endptr, 0); // Nombre de threads à utiliser
    if (*endptr != '\0' || nb_thread <= 0) {
        fprintf(stderr, "Le nombre de threads doit être un nombre entier supérieur à 0\n");
        return 1;
    }

    pthread_t *tableau_id_thread = NULL ;
    tableau_id_thread = malloc(sizeof(pthread_t) * (nb_thread)) ;
    if (tableau_id_thread == NULL) {
        fprintf(stderr, "Erreur lors de l'allocation");
    }

    work_list = malloc(sizeof(bool)*(maximum+1) ); // Allocation de la liste de travail
    if (work_list == NULL) {
        fprintf(stderr, "Erreur lors de l'allocation");
    }

    // Création de l'ensemble des threads, qui exécuteront la méthode `do_work`.
    for (i=0; i!= nb_thread; i++){
        depart_argument = i+2 ;
        pthread_create( tableau_id_thread + i, NULL , do_work, (void *) depart_argument ) ;

    }

    // Attendre que l'ensemble des threads soit terminé.
    for (i=0; i!= nb_thread; i++){
        pthread_join( tableau_id_thread[i], NULL ) ;
    }

    // Comptage du nombre de nombres premiers trouvés
    for(i=2; i <= maximum; i++) {
        if(work_list[i]){
            nb++;
        }
    }

    printf("Nombre de nombres premiers trouvés : %d\n", nb) ;

    return 0;
}

/*
 * From `man time`
 *        The resource specifiers, which are a superset of those recognized by the tcsh(1) builtin
       `time' command, are:
              %      A literal `%'.
              C      Name and command line arguments of the command being timed.
              D      Average size of the process's unshared data area, in Kilobytes.
              E      Elapsed real (wall clock) time used by the process, in
                     [hours:]minutes:seconds.
              F      Number of major, or I/O-requiring, page faults that occurred while the
                     process was running.  These are faults where the page has actually migrated
                     out of primary memory.
              I      Number of file system inputs by the process.
              K      Average total (data+stack+text) memory use of the process, in Kilobytes.
              M      Maximum resident set size of the process during its lifetime, in Kilobytes.
              O      Number of file system outputs by the process.
              P      Percentage of the CPU that this job got.  This is just user + system times
                     divided by the total running time.  It also prints a percentage sign.
              R      Number of minor, or recoverable, page faults.  These are pages that are not
                     valid (so they fault) but which have not yet been claimed by other virtual
                     pages.  Thus the data in the page is still valid but the system tables must
                     be updated.
              S      Total number of CPU-seconds used by the system on behalf of the process (in
                     kernel mode), in seconds.
              U      Total number of CPU-seconds that the process used directly (in user mode),
                     in seconds.
              W      Number of times the process was swapped out of main memory.
              X      Average amount of shared text in the process, in Kilobytes.
              Z      System's page size, in bytes.  This is a per-system constant, but varies
                     between systems.
              c      Number of times the process was context-switched involuntarily (because the
                     time slice expired).
              e      Elapsed real (wall clock) time used by the process, in seconds.
              k      Number of signals delivered to the process.
              p      Average unshared stack size of the process, in Kilobytes.
              r      Number of socket messages received by the process.
              s      Number of socket messages sent by the process.
              t      Average resident set size of the process, in Kilobytes.
              w      Number of times that the program was context-switched voluntarily, for
                     instance while waiting for an I/O operation to complete.
              x      Exit status of the command.
 *
 */
