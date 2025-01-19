Programme “Hello, World!” en C
```c
#include <stdio.h>

int main(){
	printf("Hello, World!\n");
	return 0;
}
```
l’appel système write utilisé pour afficher le Hello, World!.
strace -e trace=write ./Hello_World_c

Programme “Hello, World!” en C++
```c
#include <iostream>

int main(){
    std::cout << "Hello, world!\n";
    return 0;
}
```

l’appel système write utilisé pour afficher le Hello, World!.
```
strace -e trace=write ./Hello_World_cpp
```

Programme “Hello, World!” en Bash
```
#!/bin/bash

echo "Hello, World!"
```
l’appel système write utilisé pour afficher le Hello, World!.
````
strace -e trace=write ./Hello_World_bash.sh
````
Programme “Hello, World!” en Python
````
print("Hello, World!")
````
l’appel système write utilisé pour afficher le Hello, World!.
````
strace -e trace=write python ./Hello_World_python.py
````

### Conclusion : il y a nécessairement des appels système effectués pour réussir à afficher Hello, World! 
### Il n’y a pas de passe-droits ou de contournement possible, peu importe le langage de programmation utilisé.
#### Tubes et redirections
````
strace ./Hello_World_c > fichier
strace ./Hello_World_c | cowsay
````
### On remarque que ni la redirection ni l’utilisation des tubes n’impactent l’utilisation des appels système par les programmes.
#### Variété
````
strace -tf sleep 5
strace -c sleep 5 
````
affiche 32 appels système pour sleep 5
sleep 5 ne fait ni calcul ni entrées-sorties
Les appels système les plus utilisés sont : “mmap” et “close”
````
strace -tf ./prime 100
strace -c ./prime 100 
````
affiche 37 appels système pour ./prime
./prime fait plus de calcul qu’entrées-sorties
Les appels système les plus utilisés sont : “mmap” et “mprotect”
````
strace -tf find /usr/lib
strace -c find /usr/lib affiche 77356 appels système pour find /usr/lib
````
find /usr/lib fait plus d’entrées-sorties que de calcul
Les appels système les plus utilisés sont : “write”, “fcntl” et “close”
````
strace -tf xeyes
strace -c xeyes affiche 2947 appels système pour xeyes
````
xeyes fait plus d’entrées-sorties que de calcul
Les appels système les plus utilisés sont : “recvmsg” et “poll”
````
strace -tf mpv mamusique.mp3
strace -c mpv mamusique.mp3 affiche 7763 appels système pour mpv mamusique.mp3 pour une durée de 15 secondes
````
mpv mamusique.mp3 fait plus de calcul que d’entrées-sorties
Les appels système les plus utilisés sont : “futex” et “getpid”
### Les programmes interactifs qui utilisent beaucoup d’entrées-sorties font beaucoup d’appels systèmes
### Programme prime fait beaucoup de calcul et utilise peu d’appels système, et aussi la commande sleep qui ne fait ni calcul ni entrées-sorties.
