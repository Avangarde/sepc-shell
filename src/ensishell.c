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
#include <fcntl.h>
#include <unistd.h>


#include "variante.h"
#include "readcmd.h"

#ifndef VARIANTE
#error "Variante non défini !!"
#endif

Liste processus;

void asynchro(int signal) {
    
    Liste parcours = processus; 
    pid_t pid;
    
    while (!estVide(parcours)) {
        pid = waitpid (parcours->pid, NULL, WNOHANG);
        if (pid >0){
            //printf("Changement d'état de processus : %i\n", pid);
            parcours->etat = 1;
        }else{
            
        }
        
        parcours = parcours->suivant;
        
    }
}

void lancerCommande(struct cmdline * cmdl) {

    // Morceau du code du sigaction
    struct sigaction traitant = {};
    traitant.sa_handler = asynchro;
    sigemptyset(& traitant.sa_mask);
    traitant.sa_flags = 0;
    if (sigaction(SIGCHLD, &traitant, 0) == -1){
        perror("sigaction:");
    }


    //Variables PIPE
    int tuyau[2];
    int pids[2];
    int status;
    //tuyau[0]: lire... ecrire sur l'autre
    pipe(tuyau);

    //Ici il manque le trc pour faire la lecture du pipe bien si nest pas selement le premier
    int s;
    for (s = 0; cmdl->seq[s] != 0; s++) {
        char **commande = cmdl->seq[s];
        if (strcmp(commande[0], "quit") == 0
                || strcmp(commande[0], "exit") == 0) {
            exit(0);
        }

        if (strcmp(commande[0], "jobs") == 0) {
            processus = visualiser(processus);
            
            
        } else {
            pid_t pid;
            switch (pid = fork()) {
                case -1:
                    perror("fork:");
                    break;
                case 0:
                    //Est-ce que je suis la premiere sequence?
                    if (s == 0) {
                        //Est-ce que je suis l'unique sequence
                        if (cmdl->in != NULL) {
                            //Traitement
                            //J'essai d'ouvrir le standard in en mode read-only
                            int stdIn;
                            if ((stdIn = open(cmdl->in, O_RDONLY)) == -1) {
                                //Traitement d'erreur:On ne peut pas ouvrir
                                perror("open:");
                                exit(1);
                            }

                            dup2(stdIn, 0);
                            if (close(stdIn) == -1) {
                                //On ne peut pas le fermer
                                perror("close:");
                            }
                        }
                        //Ici on doit regarder si on est l'unique commande ou s'il ya plusieurs
                        if (cmdl->seq[s + 1] == 0) {
                            if (cmdl->out != NULL) {
                                int stdOut;
                                if ((stdOut = open(cmdl->out, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO)) == -1) {
                                    perror("open:");
                                    exit(1);
                                }

                                dup2(stdOut, 1);
                                if (close(stdOut) == -1) {
                                    perror("close:");
                                }
                            }


                        } else {

                            dup2(tuyau[1], 1);
                            //Attention, est qu'il faut fermer l'ecriture avant de fermer la lecture?
                            close(tuyau[1]);
                            close(tuyau[0]);

                        }
                    }//Est-ce que je suis la deuxieme sequence?

                    else if (s == 1) {

                        dup2(tuyau[0], 0);
                        close(tuyau[0]);
                        close(tuyau[1]);

                        //Gestion du fichier	out

                        if (cmdl->out != NULL) {
                            int stdOut;
                            if ((stdOut = open(cmdl->out, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO)) == -1) {
                                perror("open:");
                                exit(1);
                            }

                            dup2(stdOut, 1);
                            if (close(stdOut) == -1) {
                                perror("close:");
                            }
                        }
                    }

                    execvp(commande[0], commande);
                    perror("execvp:");
                    exit(0);
                    break;
                default:
                {
                    if (!cmdl->bg) {
                        pids[s] = pid;
                        if (s == 0) {
                            waitpid(pid, &status, 0);
                        }
                    } else {
                        printf("Processus en tache de fond: %i\n", pid);
                        processus = ajouterAuFond(pid, cmdl->seq[s], processus);
                    }
                    break;
                }
            }
        }
    }


    //Verifier avant de sortir que les pipes sont fermes
    close(tuyau[0]);
    close(tuyau[1]);
    waitpid(pids[0], &status, 0);
    waitpid(pids[1], &status, 0);

}

void montrerPipe(struct cmdline * cmdl) {
    int i, j;
    for (i = 0; cmdl->seq[i] != 0; i++) {
        char **cmd = cmdl->seq[i];
        printf("seq[%d]: ", i);
        for (j = 0; cmd[j] != 0; j++) {
            printf("'%s' ", cmd[j]);
        }
        printf("\n");
    }

}

int main() {
    printf("Variante %d: %s\n", VARIANTE, VARIANTE_STRING);

    while (1) {
        struct cmdline *l;
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
        montrerPipe(l);


        //lancerCommande(l->seq, l->bg);
        lancerCommande(l);

    }

}

