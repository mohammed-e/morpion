/* Mohammed EL KHAIRA */
#ifndef IA_H
#define IA_H

#include "grille_t.h"


/**************************************************************/
/* Définition de la structure représentant la mémoire de l'IA */
/**************************************************************/


typedef grille_t memoire[TAILLEMEMIA];	/* tableau représentant notre Morpion ie permettant de stocker la mémoire de l'IA */



/******************************************************************/
/* Prototypes des fonctions de manipulation de la mémoire de l'IA */
/******************************************************************/


/* Initialise le Morpion, ie le tableau mem servant à stocker l'ensemble des grilles de jeu possibles 
	<=> initialise les 19683 grilles avec la fonction grille_initialiser */
void ia_initialiser(memoire mem);


/* Vérifie si la grille actuelle g existe en mémoire (id de la grille dans mem et id de g doivent être égaux) */
uint32_t ia_id_existe(memoire mem, grille_t* g);

/* Renvoie la grille en mémoire dont l'id correspond à la grille actuelle g
	Renvoie NULL si la grille correspondante est une grille initialisée seulement (id et billeSortie égal à 0) */
grille_t* ia_id_rechercher(memoire mem, grille_t* g);


/* Ajoute à mem la configuration de la grille g actuelle rencontrée par l'IA, si elle n'existe pas encore 
	Après l'ajout, le tableau des grilles équivalentes est mis à jour pour chaque grille concernée */
void ia_grille_ajouter(memoire mem, grille_t* g);

/** Tire au sort un numéro de cases dans [1, 9] en utilisant le nb de bille comme poids et le renvoie après affectation
	Les cases déjà jouées ont un nbBille = 0 pour ne pas être tirées au sort **/
uint32_t ia_tirage_billeSortie(memoire mem, grille_t *g, uint32_t marquage);

/* Augmente ou annule le nb de billes des cases tirées au sort pour les grilles jouées, en fonction du resultat de la partie 
	(resultat = 1 : défaite IA, 2 : victoire IA ou 3 : égalité) */
void ia_renforcement(memoire mem, uint32_t resultat, uint32_t marquage);


/* Sauvegarde la mémoire de l'IA actuelle dans un fichier texte */
void ia_sauvegarder(memoire mem);

/* Charge la mémoire de l'IA des précédentes parties à partir d'un fichier texte */
void ia_charger(memoire mem, FILE *save);

#endif /* IA_H */
