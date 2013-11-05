#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "liste.h"

void visualiser (Liste L) {
	if (estVide(L)) {
		printf("Pas des processus en tâche de fond");
	} else {
		int i,j = 1;
		pid_t pid;
		Liste aux = L;
		while (!estVide(aux)) {
			printf("[%i] \t %i \t", j, aux->pid);
			pid = waitpid (aux->pid, NULL, WNOHANG);
			if (pid > 0) {
				printf("Fini");
				eliminerDeListe(aux);
			} else {
				printf("En cours d'exécution");
			}
			for (i=0; aux->commande[i]!=0; i++) {
				printf("%s ", commande[i]);
            }
            aux = aux->suivant;
            printf("\n");
		}
	}
}

int estVide(Liste L) { 
	return L == NULL; 
}

int ajouterAuFond(int pid, char **commande, Liste l){

	Liste nouvelleProc;
	nouvelleProc->pid = pid;
	nouvelleProc->commande = commande;
	nouvelleProc->suivant = NULL;
	
	if(estVide(l)){
		l = nouvelleProc;
		return 0;
	}
	
	while (l->suivant!=NULL){
		l = l->suivant;
	}
	l->suivant = nouvelleProc;
	return 0;
}
