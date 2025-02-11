# Comparaison de `chrt` et `nice` dans la Gestion des Processus Linux

## Concepts de Base

### Ordonnancement de Processus sous Linux
Le noyau Linux gère l’ordonnancement des processus via deux mécanismes principaux :
- **Valeurs Nice** : Système de priorité à l’ancienne (style Unix) pour les tâches non temps réel
- **Priorités Temps Réel** : Ordonnancement déterministe pour les opérations critiques en temps

## Tableau Comparatif des Commandes

| Fonctionnalité            | `nice`                                          | `chrt`                                                      |
|---------------------------|-------------------------------------------------|-------------------------------------------------------------|
| **Objectif Principal**    | Ajuster la priorité CPU dans l’ordonnanceur standard  | Gérer les politiques et priorités d’ordonnancement temps réel |
| **Plage de Priorité**     | -20 (plus haute) à +19 (plus basse)            | 1 à 99 pour les politiques temps réel                       |
| **Valeur Par Défaut**     | 0                                               | Dépend de la politique (généralement 0 pour SCHED_OTHER)    |
| **Politiques d’Ordonnancement** | SCHED_OTHER (CFS) uniquement               | SCHED_FIFO, SCHED_RR, SCHED_OTHER, SCHED_BATCH, SCHED_IDLE  |
| **Droits Root Requis**    | Seulement pour des valeurs négatives           | Requis pour les politiques temps réel                       |
| **Impact sur le Processus** | Influence la part CPU de manière proportionnelle  | Peut fournir une priorité CPU absolue                       |
| **Risque Système**        | Faible – Ne peut pas monopoliser le CPU         | Élevé – Peut bloquer le système si mal configuré            |

## Guide Pratique d’Utilisation

### Utiliser `nice`

#### Opérations de Base
```bash
# Lancer un nouveau processus avec une valeur nice modifiée
nice -n 10 ./backup_script.sh    # Priorité plus basse
nice -n -10 ./critical_task.sh   # Priorité plus haute (nécessite root)

# Modifier la priorité d’un processus existant
renice -n 5 -p 1234              # Rendre le processus 1234 plus “gentil”
sudo renice -n -5 -p 1234        # Rendre le processus 1234 moins “gentil” (priorité plus haute)

# Vérifier la valeur nice actuelle
nice                             # Affiche la valeur nice du processus courant
ps -o pid,nice,comm -p $$        # Affiche la valeur nice pour le shell courant
```

#### Utilisation Avancée de Nice
```bash
# Plusieurs processus avec la même valeur nice
nice -n 15 bash -c 'command1 & command2 & command3'

# Héritage de la valeur nice
nice -n 10 bash
# Toutes les commandes dans ce shell héritent d’une valeur nice de 10

# Modification d’un groupe de processus
renice -n 5 -g $(ps -o pgid -p $$ | tail -1)
```

### Utiliser `chrt`

#### Opérations de Base
```bash
# Afficher la politique d’ordonnancement et la priorité actuelles
chrt -p $$                      # Affiche pour le shell courant
chrt -p 1234                    # Affiche pour le processus 1234

# Définir l’ordonnancement FIFO en temps réel
sudo chrt -f 80 ./rt_process    # Lancer avec politique FIFO, priorité 80
sudo chrt -f -p 80 1234         # Changer un processus existant en FIFO

# Définir l’ordonnancement Round Robin en temps réel
sudo chrt -r 60 ./rt_process    # Lancer avec politique RR, priorité 60
sudo chrt -r -p 60 1234         # Changer un processus existant en RR
```

#### Ordonnancement Temps Réel Avancé
```bash
# Lister toutes les politiques d’ordonnancement disponibles
chrt -m                         # Affiche les priorités min/max pour chaque politique

# Définir l’ordonnancement batch
sudo chrt -b -p 0 1234          # Politique SCHED_BATCH

# Définir l’ordonnancement idle
sudo chrt --idle -p 0 1234      # Politique SCHED_IDLE

# Groupe de tâches temps réel complexe
sudo chrt -f 80 bash -c '
  ./critical_task &             # Hérite de FIFO-80
  chrt -f -p 70 $!              # Priorité inférieure pour la tâche de fond
  ./main_task                   # S’exécute avec FIFO-80 d’origine
'
```

## Bonnes Pratiques

### Quand Utiliser `nice`
1. **Tâches intensives en CPU en arrière-plan**
   ```bash
   nice -n 19 tar czf backup.tar.gz /home/user/data/
   ```

2. **Multiples processus concurrents**
   ```bash
   nice -n 10 make -j$(nproc) &    # Compilation
   nice -n 15 ffmpeg ... &         # Encodage vidéo
   ```

### Quand Utiliser `chrt`
1. **Traitement audio**
   ```bash
   sudo chrt -f 80 jackd -d alsa   # Serveur audio
   sudo chrt -f 75 ardour          # Logiciel DAW
   ```

2. **Acquisition de données en temps réel**
   ```bash
   sudo chrt -f 99 ./sensor_reader  # Priorité temps réel maximale
   sudo chrt -f 90 ./data_processor # Priorité légèrement inférieure
   ```

## Considérations de Sécurité

### Sécurité avec Nice
- Ne peut pas monopoliser les ressources système
- Sans danger pour un usage courant
- Les utilisateurs non-root sont limités aux valeurs positives
```bash
# Tâche de fond sûre
nice -n 19 ./intensive_task.sh &
```

### Sécurité avec Chrt
- Peut potentiellement bloquer le système
- Nécessite un plan de priorité rigoureux
- Toujours tester avec des priorités plus basses d’abord
```bash
# Approche de test sécurisé
sudo chrt -f 50 ./rt_task    # Commencer avec une priorité moyenne
# Surveiller la réactivité du système avant d’augmenter
```

## Surveillance et Débogage

### Surveillance de Nice
```bash
# Surveiller les valeurs nice
watch -n 1 'ps -eo pid,ppid,ni,comm | grep -v "^[[:space:]]*$"'

# Trouver les processus par valeur nice
ps -eo pid,ppid,ni,comm | awk '$3 > 10'
```

### Surveillance de Chrt
```bash
# Surveiller les processus temps réel
watch -n 1 'ps -eo pid,cls,rtprio,comm | grep -v "^[[:space:]]*$"'

# Lister tous les processus temps réel
ps -eo pid,cls,rtprio,comm | grep -E '^[[:space:]]*[0-9]+[[:space:]]+(FF|RR)'
```

## Problèmes Courants et Solutions

### Problèmes avec Nice
1. **Permission non accordée**
   ```bash
   # Solution : Utiliser sudo pour des valeurs négatives
   sudo nice -n -10 ./important_task
   ```

2. **Héritage de Priorité**
   ```bash
   # Solution : Utiliser exec pour éviter l’héritage du shell
   nice -n 19 exec ./background_task
   ```

### Problèmes avec Chrt
1. **Opération non permise**
   ```bash
   # Solution : Vérifier les capacités adéquates
   sudo setcap cap_sys_nice+ep ./rt_program
   ```

2. **Système non réactif**
   ```bash
   # Récupération : SSH depuis une autre machine
   ssh user@host "sudo chrt -o -p 0 FROZEN_PID"
   ```

## Cas Concrets

### Services Système
```bash
# Serveur de base de données avec priorité contrôlée
sudo systemctl edit postgresql
# Ajouter :
[Service]
Nice=-5
# Ou pour du temps réel :
CPUSchedulingPolicy=fifo
CPUSchedulingPriority=80
```

### Traitement Multi-niveaux
```bash
#!/bin/bash
# Lancer un service critique avec priorité temps réel
sudo chrt -f 80 ./critical_service &
CRIT_PID=$!

# Lancer un service important mais non temps réel
sudo nice -n -10 ./important_service &
IMP_PID=$!

# Lancer un service en arrière-plan
nice -n 19 ./background_service &
BG_PID=$!

# Surveiller tous les services
tail --pid=$CRIT_PID -f /dev/null