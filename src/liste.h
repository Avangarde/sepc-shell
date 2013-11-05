#ifndef __LISTE_H
#define __LISTE_H

struct Liste{
    pid_t pid;
    char **commande;
    struct Liste * suivant;         
};

void visualizer(Liste l);
//elimination de liste

int ajouterAuFond(int pid, char **commande, Liste l);
//vide traiter sur le code avec un variable
int eliminerDeListe(int pid, Liste l);

//TODO validation job
int estVide(Liste l);

#endif
