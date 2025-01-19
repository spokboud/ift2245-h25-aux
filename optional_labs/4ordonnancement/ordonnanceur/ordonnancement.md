# Ordonnancement
#### Afin de donner une valeur de gentillesse au programme prime_th on utilise la commande suivante nice -n 10 prime_th 10000000 10 &, n’hésitez pas à donner au programme des valeurs grandes pour vous laisser le temps de voir votre processus avec ps -l
#### Pour lancer les 3 commandes en même temps avec l’affichage du temps d’exécution(réel, mode utilisateur, mode noyau), ainsi que le nombre d’interuption volontaire et involontaire, on utilisera la commande suivante:
````
/usr/bin/time --format 'n19: %e %U %S %w %c' nice -n 19 ./prime_th 50000000 10 & \
/usr/bin/time --format 'n00: %e %U %S %w %c' ./prime_th 50000000 10 & \
/usr/bin/time --format 'idle: %e %U %S %w %c' chrt --idle 0 ./prime_th 50000000 10 &
````
#### On remarquera que le temps d’exécution augmente proportionnellement avec la valeur de la gentillesse, car plus un processus est “gentil” plus il est succeptible de laisser la ressource processeur aux autres processus. (La politique IDLE est plus faible qu’une valeur de gentillesse de 19)

### Question facultative
````
/usr/bin/time --format 'round-robin: %e %U %S %w %c' sudo chrt --rr 50 ./prime_th 50000000 10 & \
/usr/bin/time --format 'n -20: %e %U %S %w %c' sudo nice -n -20 ./prime_th 50000000 10 &
````
#### Simulation d’ordonnancement
### FIFO
#### Processus\Unité de temps	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16
#### A	*	*	*	*	*												
#### B		-	-	-	-	*	*	*									
#### C			-	-	-	-	-	-	*	*	*	*	*	*	*	*	*
### FIFO avec préemtion avec priorité
#### Processus\Unité de temps	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16
#### A	*	-	-	-	-	-	-	-	-	-	-	-	-	*	*	*	*
#### B		*	*	*													
#### C			-	-	*	*	*	*	*	*	*	*	*				
### FIFO sans préemption avec priorité
#### Processus\Unité de temps	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16
#### A	*	*	*	*	*												
#### B		-	-	-	-	*	*	*									
#### C			-	-	-	-	-	-	*	*	*	*	*	*	*	*	*
### RR avec priorité
#### Processus\Unité de temps	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16
#### A	*	-	-	-	-	-	-	-	-	-	-	-	-	*	*	*	*
#### B		*	*	*													
#### C			-	-	*	*	*	*	*	*	*	*	*				
### RR sans priorité
#### Processus\Unité de temps	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16
#### A	*	*	-	-	-	-	*	*	-	-	-	*					
#### B		-	*	*	-	-	-	-	*								
#### C			-	-	*	*	-	-	-	*	*	-	*	*	*	*	*
### SJF
#### Processus\Unité de temps	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16
#### A	*	*	*	*	*												
#### B		-	-	-	-	*	*	*									
#### C			-	-	-	-	-	-	*	*	*	*	*	*	*	*	*
### SRT
#### Processus\Unité de temps	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16
#### A	*	-	-	-	*	*	*	*									
#### B		*	*	*													
#### C			-	-	-	-	-	-	*	*	*	*	*	*	*	*	*
#### Le temps moyen d’exécution et temps moyen d’attente
#### Stratégie d’ordonnancement	Temps moyen d’exécution	Temps moyen d’attente
````
FIFO	(5+7+15)/3 = 9	(4+6)/3 = 3,3
FIFO avec préemption avec priorité	(17+3+11)/3 = 10,3	(12+2)/3 = 4,6
FIFO sans préemption avec priorité	(5+7+15)/3 = 9	(4+6)/3 = 3,3
RR avec priorité	(17+3+11)/3 = 10,3	(12+2)/3 = 4,6
RR sans priorité	(12+8+15)/3 = 11,6	(7+5+6)/3 = 6
SJF	(5+7+15)/3 = 9	(4+6)/3 = 3,3
SRT	(8+3+15)/3 = 8,6	(6+3)/3 = 3
````
### État des processus
|Progs|Processus\Unité de temps| | | | | | | | | | | | | | | | | | | |
|:---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---:|
| |0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|
|A|	actif|	actif|	bloqué|	bloqué|	prêt|	prêt|	prêt|	prêt|	prêt|	prêt|	prêt|	prêt|	prêt|	prêt|	actif|	actif|	actif|	actif|		
|B|		prêt|	actif|	actif|	actif|	actif|	bloqué|	prêt|	prêt|	prêt|	prêt|	prêt|	prêt|	prêt|	prêt|	prêt|	prêt|	prêt|	actif|	actif|
|C|			prêt|	prêt|	prêt|	prêt|	actif|	actif|	actif|	actif|	actif|	actif|	actif|	actif|					
