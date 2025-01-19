#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAXDONNEE 1 // Nombre maximal de données produites par chaque producteur

void attendre() {
  int sleep_time = (rand() % 5) + 1;
  // int sleep_time = 1;
  sleep(sleep_time);
}

// La déclaration des deux sémaphores permettant de savoir s'il y a une donnée disponible pour le consommateur et si le producteur doit produire une donnée
sem_t empty;
sem_t full;

// Les mutex pour éviter les accès concurrent à l'index d'écriture et lecture.
pthread_mutex_t mutex_prod = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cons = PTHREAD_MUTEX_INITIALIZER;

#define TAILLE_MAX 10
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

    // Gestion de la sémaphore
    sem_wait(&empty);
    pthread_mutex_lock(&mutex_prod);

    int j = index_ecriture;
    index_ecriture++;
    if (index_ecriture == TAILLE_MAX)
      index_ecriture = 0;

    // Génération de la donnée
    sprintf(liste[j], "Le producteur %d a créé la donnée %d", id_producteur, i);
    printf("(+) Le producteur %d a produit : \n\tLe producteur %d a créé la donnée %d\n",id_producteur ,id_producteur, i);

    attendre();

    // Gestion de la sémaphore
    pthread_mutex_unlock(&mutex_prod);
    sem_post(&full);
  }
}

void* consommateur(void *id) {
  int id_consommateur = *((int *) id);

  // Calcul du nombre de données à consommer pour chaque consommateur
  int nb_a_consommer = TOTALDONNEE / nombre_conso + (TOTALDONNEE % nombre_conso > id_consommateur);

  for (int i = 0; i < nb_a_consommer; i++) {
    // Gestion de la sémaphore
    sem_wait(&full);
    pthread_mutex_lock(&mutex_cons);

    int j = index_lecture;
    index_lecture++;
    if (index_lecture == TAILLE_MAX)
      index_lecture = 0;

    printf ("(-) Consommateur %d a consommé la donnée : \n\t\"%s\"\n", id_consommateur, liste[j]);

    // Gestion de la sémaphore
    pthread_mutex_unlock(&mutex_cons);
    sem_post(&empty);
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

  // Initialisez les sémaphores
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, TAILLE_MAX);

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

  // Création de l'ensemble des threads, qui exécuteront la fonction producteur
  for (i=0; i!= nombre_produ; i++){
    arg_pro[i] = i;
    pthread_create( prod_thread + i, NULL , producteur, &arg_pro[i]) ;
  }

  // Création de l'ensemble des threads, qui exécuteront la fonction consommateur
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

  // Libérer la mémoire
  free(arg_conso);
  free(arg_pro);
  free(cons_thread);
  free(prod_thread);

  // Affiche les valeurs finales de full et empty.
  // On s'attend à 0 et 10 respectivement
  int f, e;
  sem_getvalue(&full, &f);
  sem_getvalue(&empty, &e);
  printf("Full: %d, Empty: %d\n", f, e);
  // Ajoutez la destruction des sémaphores
  sem_destroy(&full);
  sem_destroy(&empty);

  return 0;
}
