# ift2245-h25-aux  
# Cours de Systèmes d'Exploitation
#### TP1-2_Processes-Threads-Ordonnancement-Synchronisation-Interblocage
- [TP0](https://github.com/IFT2245/Course-TP0)  

`strace ./program 2>&1 | grep -E 'read|write'`

|||
|--------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------|
| `strace`                 | Trace tous les **appels système** (e.g. `read`, `write`, `open`, etc.) effectués par le programme.                                                       |
| `./program`              | Programme à exécuter et analyser.                                                                                                                        |
| `2>&1`                   | Redirige le **flux d’erreurs** (`2`) vers la **sortie standard** (`1`). <br/> Le **`&`** indique la référence à un **descripteur de fichier** (ici `1`). |
| `|` (Pipe)               | Transfère la **sortie standard** de la commande précédente (après redirection) comme **entrée** de la commande suivante.                                 |
| `grep -E 'read\|write'`  | Filtre les lignes pour n’afficher que celles contenant **`read`** ou **`write`**, grâce à une **expression régulière étendue** (option `-E`).            |

**Résultat** : Affiche uniquement les appels système `read` et `write` émis par `./program`.


|:---|---:|
|Extra|https://overthewire.org/wargames/bandit/|
|Extra|cat /usr/share/vim/vim91/tutor/tutor.fr.utf-8|
|Extra|https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html|
|Extra|https://waytolearnx.com/2018/08/qcm-linux-gestion-de-processus.html|
|Extra|man glob.7|
|Extra|man regex.7|
|Extra|man man|

### Update MAN
````
sudo apt install man-db manpages-posix
sudo apt install manpages-dev manpages-posix-dev
````

### Verifier la version de la libc
```
dpkg -l libc6
apt-cache policy libc6
```

### Permissions Spéciales pour les fichiers: 

| **Commande**                                        | **Usage / Description**                                                                               |
|-----------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| `chmod u+s <fichier>`                               | Active le **SUID** : le fichier s’exécute avec l’UID du propriétaire (souvent `root`).                |
| `chmod g+s <dossier>`                               | Active le **SGID** : héritage du GID pour les fichiers créés dans ce dossier.                         |
| `chmod +t <dossier>`                                | Active le **Sticky Bit** : seuls le propriétaire (ou root) peuvent supprimer/renommer leurs fichiers. |
| `chmod 4755 <fichier>`                              | SUID + permissions (4 = SUID, 7 = rwx user, 5 = rx group, 5 = rx others).                             |
| `chmod 2755 <fichier>`                              | SGID + permissions (2xxx).                                                                            |
| `chmod 1777 <dossier>`                              | Sticky Bit + permissions (1xxx).                                                                      |
| `find / -perm -4000 -type f 2>/dev/null`            | Trouve tous les binaires **SUID** (pour vérifier sécurité).                                           |
| `setfacl -m u:alice:rw <fichier>`                   | Assigne des droits ACL à `alice` (lecture/écriture).                                                  |
| `getfacl <fichier>`                                 | Affiche les ACL existantes d’un fichier ou dossier.                                                   |
| `chattr +i <fichier>`                               | Marque un fichier **immutable** (impossible à modifier/supprimer).                                    |
| `chattr +a <fichier>`                               | Marque un fichier **append-only** (on ne peut qu’ajouter).                                            |
| `lsattr <fichier>`                                  | Liste les attributs (i, a, etc.).                                                                     |
| `umask 0022`                                        | Définit le masque de permissions                                                                      |
| `ls -l /usr/bin/passwd`                             | Permet de voir le bit SUID (le `s` dans la colonne user).                                             |
| `getcap <fichier>`                                  | Affiche les **capabilities** (capacités) attribuées à un binaire.                                     |
| `setcap cap_net_raw+ep <fichier>`                   | Ajoute la capacité `cap_net_raw` sans recourir au SUID root.                                          |

---

# Ghidra : Tableau d'Utilisation en Ligne de Commande

| **Étape**                                   | **Commande Terminal**                                                                                         | **Description / Effet**                                                                                                                 |
|--------------------------------------------|---------------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------|
| **1. Décompresser Ghidra**                 | ```bash<br>unzip ghidra_<version>.zip -d ~/tools/ghidra<br>```                                                | Décompresse le paquet Ghidra (téléchargé sur [ghidra-sre.org](https://ghidra-sre.org/)) dans un dossier (ex. `~/tools/ghidra`).         |
| **2. Accéder au Dossier**                  | ```bash<br>cd ~/tools/ghidra/ghidra_<version>/<dossier_ghidra><br>```                                         | Se place dans le dossier contenant les scripts de Ghidra (incluant `ghidraRun`, `analyzeHeadless`, etc.).                               |
| **3. Lancer Ghidra (GUI)**                 | ```bash<br>./ghidraRun<br>```                                                                                 | Ouvre l’interface graphique de Ghidra (CodeBrowser, Decompilation, etc.).                                                               |
| **4. Créer un Projet (mode headless)**     | ```bash<br>./analyzeHeadless /path/to/projects MyProject -new<br>```                                          | Crée un **nouveau projet** Ghidra, nommé `MyProject`, stocké dans `/path/to/projects`, sans ouvrir l’interface graphique.               |
| **5. Importer un Binaire (headless)**      | ```bash<br>./analyzeHeadless /path/to/projects MyProject -import /path/to/mon_binaire -analysis<br>```        | Importe et lance automatiquement l’**analyse** du binaire `mon_binaire` dans le projet `MyProject`.                                     |
| **6. Script Automatique (headless)**       | ```bash<br>./analyzeHeadless /path/to/projects MyProject -postScript MonScript.py<br>```                      | Exécute le **script** Python/Java `MonScript.py` après l’analyse (permet l’automatisation, le reformatage, etc.).                       |
| **7. Exporter les Résultats (headless)**   | ```bash<br>./analyzeHeadless /path/to/projects MyProject -export /tmp/output.xml -processor x86:LE:64<br>```  | Exporte la base de connaissances (fonctions, symboles…) vers un fichier (ex. format XML).                                               |
| **8. Mettre à Jour Ghidra**                | Téléchargez la nouvelle version et répétez les étapes 1-3.                                                    | Ghidra n’a pas d’installeur officiel : la **mise à jour** s’effectue par **remplacement** du dossier et migration éventuelle du projet. |
| **9. Vérifier l’Aide**                     | ```bash<br>./analyzeHeadless --help<br>```                                                                    | Affiche les options de la commande **headless** : import, analyse, scripts, export, etc.                                                |

## Notes Rapides
- **ghidraRun** : Lance la version **GUI** de Ghidra.
- **analyzeHeadless** : Mode **script / sans interface** pour automatiser l’analyse, exécuter des scripts, exporter les résultats.
- Les **projets** Ghidra contiennent votre analyse (fonctions renommées, structures, etc.).
- Vous pouvez mixer l’usage : analyser d’abord en headless, puis ouvrir le même projet via ghidraRun.


1. [Commandes GDB](#1-commandes-gdb)  
   1.1 [Commandes GDB de Base](#11-commandes-gdb-de-base)  
   1.2 [Fonctionnalités GDB Avancées](#12-fonctionnalités-gdb-avancées)  
2. [Commandes Valgrind](#2-commandes-valgrind)  
   2.1 [Valgrind de Base (Memcheck)](#21-valgrind-de-base-memcheck)  
   2.2 [Autres Outils Valgrind (Helgrind, Massif, Callgrind)](#22-autres-outils-valgrind-helgrind-massif-callgrind)  
3. [Commandes Utiles Supplémentaires](#3-commandes-utiles-supplémentaires)  
   3.1 [Commandes Système (Processus, Surveillance)](#31-commandes-système-processus-surveillance)  
   3.2 [Commandes Réseau et Sécurité](#32-commandes-réseau-et-sécurité)  
   3.3 [Compilation et Build](#33-compilation-et-build)  
   3.4 [Gestion de Disque](#34-gestion-de-disque)  
4. [Tests Unitaires en C](#4-tests-unitaires-en-c)  
   4.1 [Installation et Utilisation de CUnit](#41-installation-et-utilisation-de-cunit)  
   4.2 [Exemple de Test Unitaire](#42-exemple-de-test-unitaire)  
   4.3 [Autres Frameworks de Test pour le C](#43-autres-frameworks-de-test-pour-le-c)  
5. [Configuration de l’Autograder GitHub](#5-configuration-de-lautograder-github)  
   5.1 [Mise en Place du Workflow GitHub Actions](#51-mise-en-place-du-workflow-github-actions)  
   5.2 [Extensions possibles](#52-extensions-possibles)  

## 1. Commandes GDB
### 1.1 Commandes GDB de Base

| **Commande**                                       | **Description**                                                                                                       |
|----------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------|
| `gdb <program_name>`                               | Lance GDB avec l’exécutable spécifié.                                                                                 |
| `run` / `r`                                        | Démarre l’exécution du programme dans GDB.                                                                            |
| `run <args>`                                       | Lance le programme avec des arguments en ligne de commande.                                                           |
| `break <function_name>` ou `b <name>`              | Place un point d’arrêt dans la fonction spécifiée.                                                                    |
| `break <file_name>:<line_number>`                  | Place un point d’arrêt à une ligne précise d’un fichier source.                                                       |
| `break <file_name>:<line_number> if <condition>`   | Point d’arrêt **conditionnel** (se déclenche uniquement si `<condition>` est vraie).                                  |
| `list` / `l`                                       | Affiche les 10 lignes suivantes du code source.                                                                       |
| `list <line_number>`                               | Affiche le code source à partir d’une ligne spécifique.                                                               |
| `list <function_name>`                             | Affiche le code source à partir d’une fonction spécifique.                                                            |
| `print <variable>` ou `p <expr>`                   | Affiche la valeur d’une variable ou d’une expression.                                                                 |
| `step` / `s`                                       | Exécute pas à pas **en entrant** dans la fonction appelée.                                                            |
| `next` / `n`                                       | Exécute pas à pas **en restant au même niveau** (ne rentre pas dans la fonction).                                    |
| `continue` / `c`                                   | Reprend l’exécution jusqu’au prochain point d’arrêt.                                                                  |
| `backtrace` / `bt`                                 | Affiche la pile d’appels (stack trace).                                                                               |
| `watch <variable>`                                 | Crée un watchpoint qui s’active à chaque fois que la variable `<variable>` change de valeur.                          |
| `set <variable> = <value>`                         | Modifie la valeur d’une variable en cours de débogage.                                                                |
| `delete <num>`                                     | Supprime un point d’arrêt spécifique.                                                                                 |
| `disable <num>` / `enable <num>`                   | Désactive/réactive un point d’arrêt sans le supprimer.                                                                |
| `quit` / `q`                                       | Quitte GDB.                                                                                                           |

### Conseils GDB (Base)
- `info locals` : Affiche les variables locales de la fonction courante.  
- `info registers` : Affiche l’état des registres CPU.  

### 1.2 Fonctionnalités GDB Avancées

| **Commande**                                 | **Description Avancée**                                                                                         |
|----------------------------------------------|-----------------------------------------------------------------------------------------------------------------|
| `record`                                     | Enregistre l’historique d’exécution permettant le “time-travel debugging”.                                      |
| `reverse-continue` ou `rc`                   | Permet de **reculer** dans l’exécution (après un `record`).                                                     |
| `thread apply all bt`                        | Affiche un backtrace pour **tous** les threads d’un processus multithreadé.                                     |
| `set follow-fork-mode <parent|child>`        | Spécifie sur quel processus (parent ou enfant) GDB doit se concentrer après un `fork()`.                         |
| `catch syscall <syscall_name>`               | Déclenche un point d’arrêt sur un **appel système** donné (ex. `open`, `read`).                                  |
| `set disassembly-flavor intel`               | Change le format d’affichage de l’assembleur en Intel plutôt qu’AT&T.                                           |
| `layout src`                                 | Active le mode TUI (text user interface) pour voir directement le code source dans le terminal.                 |
| `info threads`                               | Liste tous les threads, leur ID et leur état.                                                                    |
| `thread <num>`                               | Bascule sur le thread numéro `<num>`.                                                                            |

---

## 2. Commandes Valgrind
### 2.1 Valgrind de Base (Memcheck)

| **Commande**                                                     | **Description**                                                                                                |
|------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------|
| `valgrind <program_name>`                                        | Lance `<program_name>` sous Valgrind avec **memcheck** (outil par défaut).                                    |
| `valgrind --leak-check=full <program_name>`                      | Effectue une vérification détaillée des fuites mémoire (rapports complets).                                   |
| `valgrind --track-origins=yes <program_name>`                    | Affiche la source (origine) des valeurs non initialisées.                                                      |
| `valgrind --show-reachable=yes <program_name>`                   | Montre la mémoire “reachable” qui n’a pas été libérée (mais reste référencée).                                |
| `valgrind --error-limit=no <program_name>`                       | Désactive la limite par défaut du nombre d’erreurs affichées.                                                 |
| `valgrind --log-file=<fichier> <program_name>`                   | Enregistre la sortie de Valgrind dans `<fichier>`.                                                             |

### 2.2 Autres Outils Valgrind (Helgrind, Massif, Callgrind)

| **Commande**                                      | **Outil**     | **Usage**                                                                                           |
|---------------------------------------------------|--------------|------------------------------------------------------------------------------------------------------|
| `valgrind --tool=helgrind <program_name>`         | **Helgrind**  | Détecte les erreurs de concurrence (race conditions) dans un programme multithreadé.                |
| `valgrind --tool=massif <program_name>`           | **Massif**    | Profile l’utilisation du **tas (heap)** dans le temps (fuites, pics mémoire, etc.).                  |
| `valgrind --tool=callgrind <program_name>`        | **Callgrind** | Analyse les appels de fonctions, la performance et l’occupation CPU par fonction.                    |
| `ms_print massif.out.<pid>`                       | **Massif**    | Affiche de façon lisible un profil mémoire généré par Massif.                                        |
| `kcachegrind callgrind.out.<pid>`                 | **Callgrind** | Interface graphique pour visualiser les données de Callgrind (sous KDE ou via install sur d’autres). |

---

## 3. Commandes Utiles Supplémentaires
### 3.1 Commandes Système (Processus, Surveillance)

| **Commande**                   | **Description**                                                                                                   |
|-------------------------------|---------------------------------------------------------------------------------------------------------------------|
| `top`                         | Vue dynamique des processus et ressources (CPU, mémoire).                                                          |
| `htop`                        | Version améliorée de `top` (interface plus conviviale, barres de ressources).                                      |
| `ps` / `ps aux`               | Liste les processus en cours ; `aux` donne plus de détails (utilisateur, CPU%, mémoire%).                          |
| `ps -eLf`                      | Liste tous les threads de tous les processus (pratique pour voir le threading).                                   |
| `kill <pid>`                  | Termine un processus par son PID.                                                                                  |
| `killall <nom_process>`       | Termine tous les processus portant le nom `<nom_process>`.                                                         |
| `lsof <fichier>`              | Montre quel(s) processus utilisent `<fichier>`.                                                                    |
| `strace <program_name>`       | Trace les appels système d’un programme (débogage bas niveau).                                                     |
| `dmesg`                       | Affiche les messages du noyau (ring buffer).                                                                       |
| `vmstat 1`                    | Statistiques sur la mémoire, la pagination, l’E/S, CPU, mises à jour toutes les 1 seconde.                        |
| `free -h`                     | Montre l’utilisation RAM et swap (option `-h` pour format lisible).                                                |
| `df -h`                       | Montre l’espace disque disponible sur chaque partition montée.                                                     |
| `du -sh <dir>`                | Calcule la taille d’un répertoire `<dir>` (et sous-répertoires).                                                   |
| `journalctl -xe`              | Affiche les logs système (systemd), y compris les erreurs, avertissements, etc.                                    |
| `systemctl status <service>`  | Vérifie l’état d’un service systemd (ex. `nginx`, `docker`).                                                       |

### 3.2 Commandes Réseau et Sécurité

| **Commande**                          | **Description**                                                                                            |
|--------------------------------------|------------------------------------------------------------------------------------------------------------|
| `netstat -tulnp` ou `ss -tulnp`      | Affiche les sockets TCP/UDP en écoute (ports ouverts). (`ss` est la version moderne de `netstat`).         |
| `ifconfig` ou `ip addr show`         | Montre les interfaces réseau et leurs adresses IP (ancien: `ifconfig`, moderne: `ip`).                     |
| `ping <host>`                        | Vérifie la connectivité réseau vers `<host>`.                                                              |
| `traceroute <host>`                  | Montre les sauts (routers) jusqu’à `<host>`.                                                                |
| `tcpdump -nn -i <iface>`             | Capture les paquets réseau sur l’interface `<iface>` sans résolution de nom (`-nn`).                       |
| `nmap -A <ip>`                       | Analyse les ports ouverts, versions de services, détection d’OS etc.                                       |
| `fail2ban-client status`             | Vérifie l’état des jails Fail2ban (sécurité brute-force SSH, etc.).                                        |
| `ufw enable` / `ufw allow <port>`    | Active le pare-feu UFW (Ubuntu) et autorise un port précis.                                                |
| `iptables -L -n -v`                  | Affiche les règles du pare-feu iptables en détail.                                                          |
| `firewalld`                          | Gestionnaire de pare-feu (sur certaines distros comme Fedora/CentOS).                                      |
| `arpwatch -i <iface>`                | Surveille les modifications d’ARP sur `<iface>` pour détecter du spoofing.                                 |
| `ssh <user>@<host>`                  | Connexion sécurisée (SSH) à une machine distante.                                                           |

### 3.3 Compilation et Build

| **Commande**                                       | **Description**                                                                                             |
|----------------------------------------------------|-------------------------------------------------------------------------------------------------------------|
| `gcc <fichier>.c -o <executable> -Wall -Wextra -g` | Compile en C avec **tous** les avertissements activés (`-Wall -Wextra`) + symboles de débogage (`-g`).     |
| `clang <fichier>.c -o <executable> -fsanitize=...` | Utilise Clang avec les sanitizers (`address`, `undefined`, etc.) pour détecter des erreurs au runtime.     |
| `make`                                             | Compile selon le Makefile.                                                                                 |
| `make clean`                                       | Supprime les artefacts de build.                                                                           |
| `make <cible>`                                     | Compile une cible spécifique du Makefile.                                                                  |
| `cmake . && make`                                  | Génère un build via CMake, puis compile (dans un dossier build de préférence).                             |
| `gdb ./<executable>`                                | Lance GDB sur l’exécutable. (Prévoir `-g` lors de la compilation).                                        |

### 3.4 Gestion de Disque

| **Commande**                             | **Description**                                                                                               |
|------------------------------------------|---------------------------------------------------------------------------------------------------------------|
| `fdisk -l`                               | Liste les partitions en style MBR.                                                                            |
| `parted /dev/sda print`                  | Liste ou modifie la table de partitions (GPT/MBR).                                                            |
| `mount <device> <mount_point>`           | Monte un périphérique sur un point de montage (ex : `mount /dev/sdb1 /mnt`).                                 |
| `umount <mount_point>`                   | Démontage du périphérique précédemment monté.                                                                 |
| `lsblk`                                  | Affiche la hiérarchie des périphériques block.                                                                |
| `resize2fs <device>`                     | Redimensionne un système de fichiers `ext2/3/4`.                                                             |
| `mkfs.ext4 /dev/sdb1`                    | Formate la partition `/dev/sdb1` en ext4 (efface les données existantes).                                    |
| `df -hT`                                 | Affiche l’espace disque disponible, formaté en mode “human-readable” + type de FS (`T`).                      |

---

## 4. Tests Unitaires en C
### 4.1 Installation et Utilisation de CUnit

1. **Installation**  
   - **Ubuntu/Debian** : `sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev`  
   - **MacOS (Homebrew)** : `brew install cunit`  

2. **Inclure dans votre projet**  
   ```c
   #include <CUnit/CUnit.h>
   #include <CUnit/Basic.h>
   ```

3. **Compilation**  
   ```bash
   gcc -o test_program test_program.c -lcunit
   ```

### 4.2 Exemple ultra basique de Test Unitaire

```c
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

// Exemple : fonction à tester
int add(int a, int b) {
    return a + b;
}

// Cas de test
void test_add() {
    CU_ASSERT_EQUAL(add(2, 3), 5);
    CU_ASSERT_EQUAL(add(-1, 1), 0);
}

int main() {
    // Initialisation
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // Création d’une suite
    CU_pSuite suite = CU_add_suite("Addition Suite", NULL, NULL);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Ajout du test
    if (NULL == CU_add_test(suite, "test add()", test_add)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Exécution
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
```

### 4.3 Autres Frameworks de Test pour le C

- **Unity** : Framework minimaliste pour microcontrôleurs et petits projets.  
- **Check** : Framework de test unitaire avec sorties TAP, XML, etc.  
- **Google Test** (gTest) : Plus orienté C++, mais on peut l’intégrer pour du C également.

---

## 5. Configuration de l’Autograder GitHub
### 5.1 Mise en Place du Workflow GitHub Actions

1. **Créer un fichier YAML** dans `.github/workflows/autograder.yml`

```yaml
name: CUnit Test Autograder

on:
  push:
    branches:
      - main

jobs:
  test:
    runs-on: ubuntu-latest

    steps:
      - name: Checkout repository
        uses: actions/checkout@v2

      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y gcc libcunit1 libcunit1-dev valgrind make

      - name: Build
        run: |
          make clean
          make

      - name: Run tests with Valgrind
        run: |
          valgrind --leak-check=full ./test_program
```

2. **Consulter les résultats** : Depuis l’onglet “Actions” du dépôt GitHub, vous verrez l’état de la compilation, des tests, et (optionnellement) des vérifications Valgrind.  

### 5.2 Extensions
- **Couverture de code** : Installer `lcov` et utiliser `gcov` pour générer des rapports de couverture.  
- **Rapports de test** : Uploader des artefacts, convertir la sortie CUnit en format JUnit/XML.  
- **GitHub Classroom** : Créer une classe, des devoirs, et relier l’autograder pour la notation automatique.

