#ifndef VARIANTE_H
#define VARIANTE_H

#define LOGINS corteshs;rojascle
#define SUJETSHA 964a91b2cb88a6d80ee63a1e41b76a2e3b240c16e7f165493f2648fdf5cff788e4370ff6405d403af9f0c64f0a6a65818edbc3c6d1b0a883a876b37d41e92f43

#define SUJET 11



#define VARIANTE SUJET

#if VARIANTE == 0
#define VARIANTE_STRING "Jokers et environnement ; Limitation du temps de calcul"
#elif VARIANTE == 1 
#define VARIANTE_STRING "Jokers étendus (tilde, brace) ; Pipes multiples"
#elif VARIANTE == 2 
#define VARIANTE_STRING "Terminaison asynchrone ; Limitations du temps de calcul"
#elif VARIANTE == 3
#define VARIANTE_STRING "Temps de calcul ; Pipes multiples"
#elif VARIANTE == 4
#define VARIANTE_STRING "Jokers et environnement ; Pipes multiples"
#elif VARIANTE == 5
#define VARIANTE_STRING "Jokers étendus (tilde, brace) ; Limitation du temps de calcul"
#elif VARIANTE == 6
#define VARIANTE_STRING "Terminaison asynchrone ; Pipes multiples"
#elif VARIANTE == 7
#define VARIANTE_STRING "Temps de calcul ; Limitation du temps de calcul )"
#elif VARIANTE == 8
#define VARIANTE_STRING "Jokers et environnement ; Terminaison asynchrone ;"
#elif VARIANTE == 9
#define VARIANTE_STRING "Jokers étendus (tilde, brace) ; Temps de calcul ;"
#elif VARIANTE == 10
#define VARIANTE_STRING "Jokers et environnement ; Temps de calcul ;"
#elif VARIANTE == 11
#define VARIANTE_STRING "Jokers étendus (tilde, brace) ; Terminaison asynchrone ;"
#endif

#endif
