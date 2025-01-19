#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NOMBRE_DE_PHILOSOPHE 5
#define NOMBRE_DE_REPAS 2

// Attends pendant une durée pseudo-aléatoire entre 1 et 5 secondes
void attendre() {
  int sleep_time = (rand() % 5) + 1;
  // int sleep_time = 1;
  sleep(sleep_time);
}

// Tableau qui contient le nombre de repas mangés par chaque philosophe.
int nombre_repas[NOMBRE_DE_PHILOSOPHE];

// Ajoutez la variable globale nécessaire pour gérer la concurrence.
int actif = 0;

// Fonction pour faire penser le philosophe pendant un temps aléatoire (entre 1 et 5s)
void pense(int id_philosophe){
  printf("Le philosophe %d pense.\n", id_philosophe);
  attendre();
}

// Fonction pour faire manger le philosophe pendant un temps aléatoire (entre 1 et 5s)
void mange(int id_philosophe){
  printf("Le philosophe %d mange.\n", id_philosophe);
  nombre_repas[id_philosophe]++;
  attendre();
}

// Cette fonction n'évite pas parfaitement les problèmes de synchronisation
void prendre_couverts(int id_philosophe){
  while (actif != 0 && actif != id_philosophe) {
    attendre();
  }
  actif = id_philosophe;
}

void poser_couverts(int id_philosophe){
	actif = 0;
}

void *philosophe(void *arg){
    int id_philosophe = *(int *)arg;
    while(nombre_repas[id_philosophe] < NOMBRE_DE_REPAS){
        pense(id_philosophe);
        prendre_couverts(id_philosophe);
        mange(id_philosophe);
        poser_couverts(id_philosophe);
    }
    printf("Le philosophe %d sort de table.\n", id_philosophe);
}

int main(int argc, char *argv[]){
  int i;

  pthread_t phi_thread[NOMBRE_DE_PHILOSOPHE];
  int tab[NOMBRE_DE_PHILOSOPHE];

  // Création de l'ensemble des threads (1 thread par philosophe), qui exécuteront la fonction philosophe
  for (i=0; i < NOMBRE_DE_PHILOSOPHE; i++){
    tab[i]=i;
    pthread_create( phi_thread + i, NULL , philosophe, &tab[i]) ;
  }

  // Attendre que l'ensemble des threads soit terminé.
  for (i=0; i < NOMBRE_DE_PHILOSOPHE; i++){
    pthread_join( phi_thread[i], NULL ) ;
  }

  return 0;
}

