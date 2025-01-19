#debug est default, un autre c'est : make BUILD=release, debug=no
BUILD = debug
cflags.common = -std=c11
cflags.debug = -W -Wall -Werror -std=c11 -pedantic -g
cflags.release = -O2 -Wall
CFLAGS = ${cflags.${BUILD}} ${cflags.common}
DEBUG=yes
CC=gcc
GG=g++

LIBS = -lm
# SRC=$(wildcard *.c)
# EXEC=$(SRC)
# OBJ=$(SRC:.c=.o)
# all: $(EXEC)

all: helloWorld_c helloWorld_cpp prime_c backup

# -Wall: Enable a set of warning, actually not all.
# -W: Enable extra warning, it's advised to use -Wextra instead which has the same meaning
# -Werror: Every warning is treated as an error.
# -ansi: On ne peut déclarer le type de l’itérateur dans l’initialisation d’une boucle for
# -Werror=vla: Initialisations de la taille des tableaux uniquement avec macro ou nombre naturel
# -std=c11: Version compilateur
# -pedantic:  Using the stricter -pedantic-errors option converts such diagnostic warnings into errors that will cause compilation to fail at such points
# -pthread: POSIX.1 specifies a set of interfaces (functions, header files) for threaded
# -lm: Lier la bibliotheque math (dynaminquement? verifier--> ```ldd executable```)
# -DLANG=FR: Préciser la langue
# -DNDEBUG: Désactiver les assertions; remplacer assert (...) par ((void)0)
# -00 || -01 || -02 || -03: optimisation

# root_dir="$(pwd)"
# -DROOT_DIR ="\" $root_dir \"": Récupérer une information dépendante du système, pwd retourne le chemin absolu du répertoire courant

# -DKERNEL ="\"$(uname)\"": Récupérer le noyau du système, uname retourne le nom du noyau


ifeq ($(DEBUG),yes)
#	LDFLAGS=-lSDL -lSDL_mixer ((Extra)Linker SimpleDirectMediaLayer)
	@echo "Génération en mode debug"
else
#	LDFLAGS=-lSDL -lSDL_mixer
	@echo "Génération en mode release"
endif

helloWorld_c: helloWorld.c
	@$(CC) $(CFLAGS) $^ -o helloWorld_c
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

# On pourrait definir une autre variable CFLAGS2
helloWorld_cpp: helloWorld.cpp
	@$(GG) $^ -o helloWorld_cpp
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

prime_c: prime.c
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

.PHONY: clean mrproper backup

clean:
	@rm -rf helloWorld_c helloWorld_cpp prime_c 2> /dev/null


mrproper: clean

backup:
	@rm -rf ./myBackUp 2> /dev/null
	@mkdir ./myBackUp 2> /dev/null
	@cp -Rp *.c ./myBackUp/