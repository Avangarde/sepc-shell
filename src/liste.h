#include <sys/types.h>
#ifndef __LISTE_H
#define __LISTE_H

typedef struct Liste{
    pid_t pid;
    char **commande;
    struct Liste * suivant;         
} Liste;

void visualizer(Liste l);
//elimination de liste

int ajouterAuFond(int pid, char **commande, Liste l);
//vide traiter sur le code avec un variable
int eliminerDeListe(Liste l);

//TODO validation job
int estVide(Liste l);

#endif
