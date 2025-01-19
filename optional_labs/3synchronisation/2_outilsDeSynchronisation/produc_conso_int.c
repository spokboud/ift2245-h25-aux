#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAXDONNEE 2 \
// Nombre maximal de données produites par chaque producteur

void attendre() {
  int sleep_time = (rand() % 5) + 1;
  // int sleep_time = 1;
  sleep(sleep_time);
}

// La déclaration des deux compteurs permettant de savoir s'il y a une donnée disponible pour le consommateur et si le producteur doit produire une donnée
int empty;
int full;


#define TAILLE_MAX 2

// Liste d'attente
char liste[TAILLE_MAX][100];
int index_ecriture=0;
int index_lecture=0;

//Nombre de consommateurs/producteurs
int nombre_produ, nombre_conso;
int TOTALDONNEE;

void* producteur(void *id) {
  int id_producteur = *((int *) id);

  for (int i = 0; i < MAXDONNEE; i++) {

    // Gestion de la disponibilité des données
	while (empty <= 0) {
      attendre();
    }
	empty--;

    int j = index_ecriture;
    index_ecriture++;
    if (index_ecriture == TAILLE_MAX)
      index_ecriture = 0;

    // Génération de la donnée
    sprintf(liste[j], "Le producteur %d a créé la donnée %d", id_producteur, i);
    printf("(+) Le producteur %d a produit : \n\tLe producteur %d a créé la donnée %d\n",id_producteur ,id_producteur, i);

	attendre();

    // Gestion de la disponibilité des données
    full++;
  }
}

void* consommateur(void *id) {
  int id_consommateur = *((int *) id);

  // Calcul du nombre de données à consommer pour chaque consommateur
  int nb_a_consommer = TOTALDONNEE / nombre_conso + (TOTALDONNEE % nombre_conso > id_consommateur);

  for (int i = 0; i < nb_a_consommer; i++) {
    // Gestion de la disponibilité des données
	while (full <= 0) {
      attendre();
    }
	full--;

    int j = index_lecture;
    index_lecture++;
    if (index_lecture == TAILLE_MAX)
      index_lecture = 0;

    printf ("(-) Consommateur %d a consommé la donnée : \n\t\"%s\"\n", id_consommateur, liste[j]);

    // Gestion de la disponibilité des données
    empty++;
  }
}

int main(int argc, char** argv) {
  int i;
  if(argc == 3){
    char* endptr = NULL;
    nombre_produ = strtol(argv[1], &endptr, 0);
    if (*endptr != '\0' || nombre_produ <= 0) {
      fprintf(stderr, "Le nombre de producteurs doit être un nombre entier supérieur à 0\n");
      return 1;
    }
    endptr = NULL;
    nombre_conso = strtol(argv[2], &endptr, 0);
    if (*endptr != '\0' || nombre_conso <= 0) {
      fprintf(stderr, "Le nombre de consommateurs doit être un nombre entier supérieur à 0\n");
      return 1;
	}
  }else{
    printf("usage : ./produc_conso nombre_producteurs nombre_consommateurs\n");
    return 1;
  }

  srand(time(NULL)); // Initialise le générateur de nombres aléatoires

  // Création et la gestion des threads
  pthread_t *prod_thread=NULL;
  pthread_t *cons_thread=NULL;
  prod_thread = malloc(sizeof(pthread_t) * (nombre_produ));
  if(prod_thread==NULL){
    perror("malloc prod_thread");
    return 1;
  }
  cons_thread = malloc(sizeof(pthread_t) * (nombre_conso));
  if(cons_thread==NULL){
    perror("malloc cons_thread");
    free(prod_thread);
    return 1;
  }

  int *arg_pro=NULL, *arg_conso=NULL;
  arg_pro = malloc(nombre_produ*sizeof(int));
  if(arg_pro==NULL){
    perror("malloc arg_pro");
    free(cons_thread);
    free(prod_thread);
    return 1;
  }
  arg_conso = malloc(nombre_conso*sizeof(int));
  if(arg_conso==NULL){
    perror("malloc arg_conso");
    free(arg_pro);
    free(cons_thread);
    free(prod_thread);
    return 1;
  }
  TOTALDONNEE = nombre_produ * MAXDONNEE;


  srand(time(NULL)); // Initialise le générateur de nombres aléatoires

  // Initialisez les compteurs
  full = 0;
  empty = TAILLE_MAX;

  // Création de l'ensemble des threads, qui exécuteront la fonction producteur
  for (i=0; i!= nombre_produ; i++){
    arg_pro[i] = i;
    pthread_create( prod_thread + i, NULL , producteur, &arg_pro[i]) ;
  }

  for (i=0; i!= nombre_conso; i++){
    arg_conso[i] = i;
    pthread_create( cons_thread + i, NULL , consommateur, &arg_conso[i]) ;
  }


  // Attendre que l'ensemble des threads soit terminé.
  for (i=0; i!= nombre_produ; i++){
    pthread_join( prod_thread[i], NULL ) ;
  }
  for (i=0; i!= nombre_conso; i++){
    pthread_join( cons_thread[i], NULL ) ;
  }

  // Affiche les valeurs finales de full et empty.
  // On s'attend à 0 et 10 respectivement
  printf("Full: %d, Empty: %d\n", full, empty);
  // Libérer la mémoire
  free(arg_conso);
  free(arg_pro);
  free(cons_thread);
  free(prod_thread);

  return 0;
}
