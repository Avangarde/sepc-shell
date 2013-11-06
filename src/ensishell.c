/*****************************************************
 * Copyright Grégory Mounié 2008-2013                *
 *           Simon Nieuviarts 2002-2009              *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


#include "variante.h"
#include "readcmd.h"

#ifndef VARIANTE
#error "Variante non défini !!"
#endif

Liste processus;

void lancerCommande (char ***seq, int bg) {
	//Variables PIPE
	int tuyau[2];
	//tuyau[0]: lire... ecrire sur l'autre
	pipe(tuyau);

	//Ici il manque le trc pour faire la lecture du pipe bien si nest pas selement le premier
	int s;
	for (s = 0; seq[s]!=0;s++){
		char **commande = seq[s];
		if(strcmp(commande[0],"quit") == 0
		   || strcmp(commande[0],"exit") == 0){
			exit(0);
		}
	
		if(strcmp(commande[0],"jobs") == 0){
			processus = visualiser(processus);
		}
		else {
			pid_t pid;
			switch( pid = fork() ) {
				case -1:
					perror("fork:");
					break;
				case 0:
					
					//Est-ce que je suis la premiere sequence?
					
					if(s==0){
					
					//Est-ce que je suis l'unique sequence
					printf("Je suis lunique");
					
						dup2(tuyau[1],1);
						//Attention, est qu'il faut fermer l'ecriture avant de fermer la lecture?
						close(tuyau[0]);
						close(tuyau[1]);
					
					}
					
					
					//Est-ce que je suis la deuxieme sequence?
				
					if(s==1){
					
						dup2(tuyau[0],0);
						close(tuyau[0]);
						close(tuyau[1]);
					
						//Gestion du fichier			
					}
				
					execvp(commande[0], commande);
					perror("execvp:");
					break;
				default:
				  { 
					int status;
					if (!bg){
						waitpid(pid, &status, 0);
					}else{
						printf("Processus en tache de fond: %i\n", pid);
						processus = ajouterAuFond(pid, seq[0], processus);
					}
					break;
				  }
			}
		}
		}
		
		
		//Verifier avant de sortir que les pipes sont fermes
		close(tuyau[0]);
		close(tuyau[1]);
		
}

int main() {
        printf("Variante %d: %s\n", VARIANTE, VARIANTE_STRING);

	while (1) {
		struct cmdline *l;
		int i, j;
		char *prompt = "ensishell>";

		l = readcmd(prompt);

		/* If input stream closed, normal termination */
		if (!l) {
			printf("exit\n");
			exit(0);
		}

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		if (l->in) printf("in: %s\n", l->in);
		if (l->out) printf("out: %s\n", l->out);
		if (l->bg) printf("background (&)\n");

		/* Display each command of the pipe */
		for (i=0; l->seq[i]!=0; i++) {
			char **cmd = l->seq[i];
			printf("seq[%d]: ", i);
                        for (j=0; cmd[j]!=0; j++) {
                                printf("'%s' ", cmd[j]);
                        }
			printf("\n");
		}
		
		lancerCommande(l->seq, l->bg);
		
	}
	
}

