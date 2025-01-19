#!/bin/bash
echo "lancement de ./pra"
./pra nohup&
echo "lancement de ./prb"
./prb nohup&
echo "lancement de ./prc"
./prc nohup&
echo "lancement de ./prd"
./prd nohup&

# option -o pour spécifier une colonne (vsz, mémoire virtuelle)
# https://fr.wikibooks.org/wiki/Fichier:Memoire_virtuelle.svg
echo "memoire pra"
ps -o vsz `pgrep pra`
echo "memoire prb"
ps -o vsz `pgrep prb`
echo "memoire prc"
ps -o vsz `pgrep prc`
echo "memoire prd"
ps -o vsz `pgrep prd`

# état du processus (actif, pret, bloqué) + des info additionnelle.
# man ps pour les détails.
# S =  sleep (une forme de bloqué),
# s = meneur de session (on n’a pas vraiment causé des sid donc je vais pas insister),
# l = processus multithread (l=light-weight process), etc.

pmap `pgrep pra`
pmap `pgrep prb`
pmap `pgrep prc`
pmap `pgrep prd`