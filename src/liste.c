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
		Liste aux = calloc(1,sizeof(*aux));
		aux->suivant = L;
		while (!estVide(aux->suivant)) {
			printf("[%i] \t %i \t", j, aux->pid);
			for (i=0; aux->suivant->commande[i]!=0; i++) {
				printf("%s ", commande[i]);
            }
			pid = waitpid (aux->suivant->pid, NULL, WNOHANG);
			if (pid > 0) {
				printf("\tFini");
				delete = aux->suivant;
				aux->suiv = aux->suiv->suiv;
				eliminerDeListe(delete);
			} else {
				printf("\tEn cours d'exécution");
				aux = aux->suivant;
			}
            printf("\n");
		}
	}
}

int estVide(Liste L) { 
	return L == NULL; 
}

int eliminerDeListe(Liste tache){
	char ** elimine = tache->command;
	int i;
	char *suppr = elimine[0];
	for(i = 1; suppr != 0; i++) {
		free(suppr);
		suppr = elimine[i];
	}
	free(elimine);
	return 1;
}
