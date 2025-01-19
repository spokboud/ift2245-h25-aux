// https://fr.wikibooks.org/wiki/Programmation_C
// https://fr.wikibooks.org/wiki/Programmation_C/Gestion_des_signaux
// https://wiki.cdot.senecacollege.ca/wiki/X86_64_Register_and_Instruction_Quick_Start
#include<signal.h>
#include<unistd.h>

int main(void) {
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = SIG_IGN;
    sigaction(SIGINT, &action, NULL);
    pause();

    return 0 ;
}
