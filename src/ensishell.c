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


#include "variante.h"
#include "readcmd.h"

#ifndef VARIANTE
#error "Variante non défini !!"
#endif


Liste processus;

void lancerCommande (char **seq, int bg) {
	//Pas testé
	if(seq[0] == 'quit'){
		exit(0);
	}
	
	if(seq[0] == 'jobs'){
		visualiser(processus);
	}
	
	pid_t pid;
	switch( pid = fork() ) {
		case -1:
			perror("fork:");
			break;
		case 0:
			execvp(seq[0], seq);
			perror("execvp:");
			break;
		default:
		  { 
			int status;
			if (!bg){
				waitpid(pid, &status, 0);
			}else{
			//Pas testé
			ajouterAuFond(pid, seq, processus);	
				}
			break;
		  }
	}
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
		
		lancerCommande(*l->seq, l->bg);
		
	}
	
}
