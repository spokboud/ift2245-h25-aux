#debug est default, un autre c'est : make BUILD=release, debug=no
BUILD = debug
cflags.common = -std=c11
cflags.debug = -g
cflags.mathThread = -pthread -lm
cflags.release = -O2 -Wall
CFLAGS = ${cflags.${BUILD}}
DEBUG=yes
CC=gcc

# LIBS =
# SRC=$(wildcard *.c)
# EXEC=$(SRC)
# OBJ=$(SRC:.c=.o)
# all: $(EXEC)

all: gestionDirecte eviterSigInt-DurantTraitement ignoreSignal traiterDeuxFoisSignal-RevenirDefault traiterUneFois-RevenirTraitementDefault verifierReception-RetarderCrtl-C shellPipe shellFifo shellFifo-2

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
# -D_DEFAULT_SOURCE: Test Macro Requirements

ifeq ($(DEBUG),yes)
#	LDFLAGS=-lSDL -lSDL_mixer ((Extra)Linker SimpleDirectMediaLayer)
	@echo "Génération en mode debug"
else
#	LDFLAGS=-lSDL -lSDL_mixer
	@echo "Génération en mode release"
endif

gestionDirecte: gestionDirecte.c
	@$(CC) $(CFLAGS) $^ -o $@
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

eviterSigInt-DurantTraitement: eviterSigInt-DurantTraitement.c
	@$(CC) $(CFLAGS) $^ -o $@
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

ignoreSignal: ignoreSignal.c
	@$(CC) $(CFLAGS) $^ -o $@
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

traiterDeuxFoisSignal-RevenirDefault: traiterDeuxFoisSignal-RevenirDefault.c
	@$(CC) $(CFLAGS) $^ -o $@
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

traiterUneFois-RevenirTraitementDefault: traiterUneFois-RevenirTraitementDefault.c
	@$(CC) $(CFLAGS) $^ -o $@
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

verifierReception-RetarderCrtl-C: verifierReception-RetarderCrtl-C.c
	@$(CC) $(CFLAGS) $^ -o $@
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

shellPipe: shellPipe.c
	@$(CC) $(CFLAGS) $^ -o $@
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

shellFifo: shellFifo.c
	@$(CC) $(CFLAGS) $^ -o $@
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

shellFifo-2: shellFifo-2.c
	@$(CC) $(CFLAGS) $^ -o $@
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore


.PHONY: clean mrproper

indent:
	@astyle  --style=allman -A2 *.c,*.h
	@rm *orig*

clean:
	@rm -rf gestionDirecte eviterSigInt-DurantTraitement ignoreSignal traiterDeuxFoisSignal-RevenirDefault traiterUneFois-RevenirTraitementDefault verifierReception-RetarderCrtl-C shellPipe shellFifo shellFifo-2 2> /dev/null

mrproper: clean

