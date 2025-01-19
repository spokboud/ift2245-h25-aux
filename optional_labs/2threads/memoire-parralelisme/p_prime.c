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
void *do_work(void *arg)
{
	long int depart = (long int)arg;
	bool is_prime= false;

	for(long int i = depart; i <= maximum; i+= nb_thread)
	{
		is_prime = true;
		// C'est inefficace, car on parcourt tous les entiers
		// au lieu de tester seulement les nombres premiers
		// mais c'est plus simple à coder (pas de synchronisation nécessaire)
		for(long int j = 2; j <= (long int )sqrt((double)i); j++)
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
	return NULL; //pour faire taire les warnings du compilo
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

	work_list = malloc(sizeof(bool)*(maximum+1) ); // Allocation de la liste de travail
	if (work_list == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation");
	}

	// Création de l'ensemble des threads, qui exécuteront la méthode `do_work`.
	pthread_t *tableau_id_thread = NULL ;
	tableau_id_thread = malloc(sizeof(pthread_t) * (nb_thread)) ;
	if (tableau_id_thread == NULL) {
		fprintf(stderr, "Erreur lors de l'allocation");
	}

	for (i = 0; i != nb_thread; i++){
		depart_argument = i+2 ;
        /*********************************************
         * arg1, store id in buffer                  *
         * arg2, default attributes                  *
         * arg3, routine                             *
         * arg4, sole argument of routine            *
         *********************************************/
         pthread_create( tableau_id_thread + i, NULL , do_work, (void*) depart_argument);
	}

//    struct thread_info {    /* Used as argument to thread_start() */
//        pthread_t thread_id;        /* ID returned by pthread_create() */
//        int       thread_num;       /* Application-defined thread # */
//        char     *argv_string;      /* From command-line argument */


	// Attendre que l'ensemble des threads soit terminé.
	for (i = 0; i != nb_thread; i++){
        /****************************************************************************************************
         * arg1, function waits for the thread specified by thread to terminate.                            *
         * arg2, If  retval  is  not  NULL, then pthread_join() copies the exit status of the target thread *
         ****************************************************************************************************/
		pthread_join( tableau_id_thread[i], NULL ) ;
	}

	// Comptage du nombre de nombres premiers trouvés
	for(i = 2; i <= maximum; i++) {
		if(work_list[i]){
			nb++;
		}
	}



	return 0;
}

//Effectivement on obtient les mêmes résultats.
//On remarque que notre programme a utilisé l’appel système clone pour créer les threads.
//On remarque que la version multithread est plus lente que la version monothread car ceci est principalement du au fait que le processeur doit gérer en plus l’aspect du parallélisme des threads.
//Conclusion : la programmation multithread n’est pas simple et n’implique pas de meilleurs performance systèmatiquement.

// time ./p_prime 11 2
// time ../2_bibliotheques-C/prime_c 11 2