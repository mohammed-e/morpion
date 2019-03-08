/* Mohammed EL KHAIRA */
#ifndef MORPION_CONSOLE_H
#define MORPION_CONSOLE_H

#include "ia.h"



/*************************************************************************/
/* Prototypes des fonctions de gestion des parties de morpion en console */
/*************************************************************************/


// création partie, affichage console ...

/* Purge le buffer en entrée pour éviter les problèmes liés à scanf */
void morpion_purger_buffer(void);

/* Permet de contrôler de la validité de la saisie de marquage (X ou O) sur la case num  
	Renvoie 0 si la saisie est correct (num dans [1,9]) et si le champ cases[num-1] vaut VIDE,
	renvoie -1 sinon */
int32_t morpion_tester_marquage(grille_t *g, uint32_t marquage, uint32_t num);


/* Affiche le menu principal et renvoie le choix de l'utilisateur */
uint32_t morpion_menu_afficher(void);

/* Affiche la demande de chargement du fichier de sauvegarde et exécute la réponse (charge ou non le fichier) */
void morpion_charger_afficher(memoire mem);

/* Affiche le résultat de la partie, la demande de rejouer et l'exécute */
void morpion_rejouer_afficher(memoire mem, grille_t *g);

/* Affiche la grille de jeu g en cours */
void morpion_grille_afficher(grille_t *g);

/* Affiche l'état de la partie puis la grille de jeu */
void morpion_afficher_jeu(grille_t* g);


/* Rempli la case de la grille g indiquée par l'utilisateur (avec une X) */
void morpion_humain_jouer(grille_t *g);

/* Rempli la case de la grille g tirée au sort par l'IA (marquage vaut X ou O) */
void morpion_ia_jouer(memoire mem, grille_t *g, uint32_t marquage, uint32_t mode);

/* Joue un coup aléatoire dans une case vide */
void morpion_ia_jouer_alea(grille_t *g, uint32_t marquage);


/* Exécute le déroulement d'une partie Joueur VS IA sans entraînement */
void mode1(memoire mem, grille_t *g);

/* Exécute le déroulement d'une partie Joueur VS IA après entraînement IA VS IA */
void mode2(memoire mem, grille_t *g, uint32_t *nbVicoireO, uint32_t *nbVicoireX, uint32_t *nbEgalite);




#endif 