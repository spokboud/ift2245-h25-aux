# Tailles des binaires
### Code source des programmes de test
Programme p0.c
```c
int main(int argv, char *argc[]){
	return 42;
}
```
Programme p1.c
```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argv, char *argc[]){
	return 42;
}
```
Programme p2.c
```c
char tableau[8000];

int main(int argv, char *argc[]){
	return 42;
}
```
Programme p3.c
```c
char tableau[8000]="hello";

int main(int argv, char *argc[]){
	return 42;
}
```
###Comparaison entre les différents programmes
````
Entre p0 et p1 : La taille du fichier p0 est égale à la taille du fichier p1 car lorsqu’on inclue des “Headers”, ceux-ci ne contiennent que des éléments tels que des prototypes de fonctions, des définitions de structures, … . Lorsqu’on ne fait appel à aucune fonction dont le prototype est dans le header, dans ce cas le compilateur ne va rien ajouter lors de la création de l’exécutable.
Entre p1 et p2 : La taille du fichier p1 peut être inférieure à la taille du fichier p2 car dans le fichier p2 nous avons déclaré une variable globale auquelle nous avons statiquement alloué un espace mémoire (BSS).
Entre p1 et p2 : La taille du fichier p12 est égale à la taille du fichier p2 car la variable tableau de 8000 bytes non initialisée et "unused" est ignorée par le compilateur
Entre p2 et p3 : La taille du fichier p2 est inférieure à la taille du fichier p3 car la valeur par laquelle nous avons initialisé le tableau sera stockée dans le fichier exécutable même si nous avons initialisé par "hello" un tableau de 8000 cases, le reste des cases est implicitement NULL.
````

```
$ du -h p0;  du -h p1;  du -h p2;  du -h p3
20K     p0
20K     p1
20K     p2
28K     p3
```

### Taille mémoire des processus
Programme qui permet d’afficher les adresses de fin de segments de texte, des données initialisées, des données non initialisées et du tas:
```c
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
```
Pour plus de détails man 3 etext (ou man 3 edata) et man 2 sbrk.

Programme pra.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char buf[1024*1024*4] = "Hello";

extern char etext, edata, end;

int main(int argv, char *argc[]){
	printf("   etext %p\n", &etext);
	printf("   edata %p\n", &edata);
	printf("   end   %p\n", &end);
	printf("   sbrk  %p\n", sbrk(0));
	pause();
	return 42;
}
```

Programme prb.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char buf[1024*1024*4] = "Hello";

extern char etext, edata, end;

int main(int argv, char *argc[]){
	printf("   etext %p\n", &etext);
	printf("   edata %p\n", &edata);
	printf("   end   %p\n", &end);
	printf("   sbrk  %p\n", sbrk(0));
	pause();
	return 42;
}
```
Programme prc.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char etext, edata, end;

int main(int argv, char *argc[]){
	char *buf = malloc(1024*1024*4);
	printf("   etext %p\n", &etext);
	printf("   edata %p\n", &edata);
	printf("   end   %p\n", &end);
	printf("   sbrk  %p\n", sbrk(0));
	pause();
	return 42;
}
```
Programme prd.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char etext, edata, end;

int main(int argv, char *argc[]){
	char buf[1024*1024*4] = "Hello";
	printf("   etext %p\n", &etext);
	printf("   edata %p\n", &edata);
	printf("   end   %p\n", &end);
	printf("   sbrk  %p\n", sbrk(0));
	pause();
	return 42;
}
```
### Comparaison de la taille en mémoire des différents programmes en utilisant ps -o vsz [pid]
#### On remarque que pour pra et prb, le fait de déclarer notre tableau statique global comme étant constant ou variable n’impacte pas la taille mais uniquement les droits sur l’espace mémoire.
#### On remarque que pour prc, on a alloué un tableau de la même taille que celui dans pra et prb mais vue qu’il est alloué dynamiquement on a besoin d’un pointeur dans lequel on va stocker l’adresse du tableau. La taille du pointeur est de 4o qui s’ajoute à l’espace mémoire nécessaire pour le programme ce qui implique que la taille de l’éxecutable a augmenté de 4Ko car la granularité est de 4Ko.
#### On remarque que pour prd, sa taille mémoire est plus petite que les autres, ceci est principalement dû au fait que le programme réutilise la mémoire libérée de la pile.

### Identification de la zone de la mémoire qui est 4Mo plus grosse, en utilisant pmap [pid]
````
Pour pra : la zone de la mémoire qui contient les 4Mo est la zone des données statiques en lecture écriture du programme.
Pour prb : la zone de la mémoire qui contient les 4Mo est la zone du code machine.
Pour prc : la zone de la mémoire qui contient les 4Mo est la zone du tas.
Pour prd : la zone de la mémoire qui contient les 4Mo est la zone de la pile.
Comparaison des adresses de pmap avec celles affichées par le programme
Pour le programme pra en triant les adresses on trouve:
557bc299a000      4K r---- pra
557bc299b000      4K r-x-- pra
557bc299b295   adresse retournée par `etext`
557bc299c000      4K r---- pra
557bc299d000      4K r---- pra
557bc299e000   4100K rw--- pra
557bc2d9e020   adresse retournée par `edata`
557bc2d9e028   adresse retournée par `end`
557bc4443000    132K rw---   [ anon ]
557bc4464000   adresse retournée par `sbrk`
7f5a193a6000    148K r---- libc-2.31.so
````
...
#### En triant les adresses on arrive bien à distinguer les différents segments de la mémoire qui sont,
````
le code du programme,
les données initialisées,
les données non initialisées(BSS) et
le tas.
````
