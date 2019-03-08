/* Mohammed EL KHAIRA */
#ifndef CASE_T_H
#define CASE_T_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#define VIDE 0
#define X 1
#define O 2
#define NBBILLEINIT 3


/*****************************************************************/
/* Définition de la structure représentant une case de la grille */
/*****************************************************************/


typedef struct case_s
{
  uint32_t num;			/* numéro de la case (joue aussi le rôle de la couleur des billes) */
  uint32_t coeffEtat;	/* vide = 0, X = 1 et O = 2 */
  uint32_t nbBille;		/* permet de symboliser le poids des numéros lors du tirage */
}case_t;



/*******************************************************************/
/* Prototypes des fonctions de manipulation des cases de la grille */
/*******************************************************************/


/* Accesseurs en lecture */
uint32_t case_get_num(case_t *c);
uint32_t case_get_coeffEtat(case_t *c);
uint32_t case_get_nbBille(case_t *c);


/* Accesseurs en écriture */
void case_set_num(case_t *c, uint32_t newNum);
void case_set_coeffEtat(case_t *c, uint32_t newCoeffEtat);
void case_set_nbBille(case_t *c, uint32_t newNbBille);


/* Manipulation */

/** Initialise la case c 
	Par défaut, la case c contient 1 bille, est vide et a pour numéro numCase (compris entre 1 à 9) **/
void case_initialiser(case_t *c, uint32_t numCase);

/** Renvoie le score de la case c (la somme des scores des cases donne l'id de la grille) 
	score = coeffEtat * 3^num **/
uint32_t case_score(case_t *c);

/* Échange 2 cases d'indice i et j d'un même tableau d'entiers */
void tab_entier_echanger(uint32_t *tab, uint32_t i, uint32_t j);

/* Échange les 2 champs coeffEtat de 2 structures cases d'indice i et j entre elles */
void cases_echanger_etat(case_t *c, uint32_t i, uint32_t j);

#endif /* CASE_T_H */


