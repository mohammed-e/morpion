/* Mohammed EL KHAIRA */
#ifndef GRILLE_T_H
#define GRILLE_T_H

#include "case_t.h"
#define XGAGNE 1
#define OGAGNE 2
#define EGALITE 3
#define ENCOURS 0

#define TAILLEMEMIA 19683 	/* = 3^9 */

/*****************************************************************/
/* Définition de la structure représentant une grille de Morpion */
/*****************************************************************/


typedef struct grille_s
{
	case_t cases[9];		/* les 9 cases de la grille pour faire le tirage et calculer l’id */
	uint32_t id;			/* indice de la grille dans le tableau */
	uint32_t jouee;				/* permet de garder une trace des grilles rencontrées dans une partie */
	uint32_t renforcee;			/* permet d'éviter que les grilles soient renforcées plusieurs fois */
	uint32_t billeSortie;	/* permet de retenir le numéro de la case jouée (renforcement) */
	uint32_t equiv[8];		/* tableau stockant l’id des grilles équivalentes dans l'ordre établie */
	uint32_t finale;		/* permet de déterminer le gagnant de la partie si terminée (0: partie en cours | 1: utilisateur | 2: IA */
}grille_t;



/***************************************************************/
/* Prototypes des fonctions de manipulation des grilles de jeu */
/***************************************************************/


/* Accesseurs en lecture */
uint32_t grille_get_id(grille_t *g);
uint32_t grille_get_jouee(grille_t *g);
uint32_t grille_get_renforcee(grille_t *g);
uint32_t grille_get_billeSortie(grille_t *g);
uint32_t* grille_get_equiv(grille_t *g);
uint32_t grille_get_finale(grille_t *g);


/* Accesseurs en écriture */
void grille_set_id(grille_t *g, uint32_t newId);
void grille_set_jouee(grille_t *g, uint32_t newJouee);
void grille_set_renforcee(grille_t *g, uint32_t newRenforcee);
void grille_set_billeSortie(grille_t *g, uint32_t newBilleSortie);
void grille_set_equiv(grille_t *g, uint32_t *newEquiv);
void grille_set_finale(grille_t *g, uint32_t newFinale);


/* Manipulation */

/** Initialise la grille g
	Par défaut, tous les champs sont nuls (jouee est à false) **/
void grille_initialiser(grille_t *g);


/** Renvoie l'id de la grille g à partir de la somme des scores des 9 cases (fonction de hachage) **/
uint32_t grille_id_calcul(grille_t *g);


/** Renvoie le numéro de la case équivalente par rotation à 90˚ 
	Il suffira d'appeler cette fonction 2 ou 3 fois pour les rotations 180˚ ou 270˚ **/
uint32_t grille_billeSortie_rot90(uint32_t billeSortie);

/** Renvoie le numéro de la case équivalente par symétrie verticale **/
uint32_t grille_billeSortie_sym(uint32_t billeSortie);


/** Renvoie la grille équivalente par rotation à 90˚, les champs à modifier étant cases et equiv
	Il suffira d'appeler cette fonction 2 ou 3 fois pour les rotations 180˚ ou 270˚ **/
grille_t grille_equiv_rot90(grille_t *g);

/** Renvoie la grille équivalente par symétrie verticale, les champs à modifier étant cases et equiv **/
grille_t grille_equiv_sym(grille_t *g);

/** Remplie le tableau equiv de la grille équivalente avec les id des grilles équivalentes **/
void grille_equiv_remplir(grille_t *g);

/** Test si une partie est terminée (3 X ou 3 O alignés)
	Renvoie XGAGNE si utilisateur gagnant, OGAGNE si IA gagnante, EGALITE en cas de match nul et ENCOURS si partie toujours en cours **/
uint32_t grille_test_finale(grille_t *g);

#endif /* GRILLE_T_H */







