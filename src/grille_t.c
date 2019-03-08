/* Mohammed EL KHAIRA */
#include "grille_t.h"



/****************************************************************/
/* Définitions des fonctions de manipulation des grilles de jeu */
/****************************************************************/


/* Accesseurs en lecture */


uint32_t grille_get_id(grille_t *g)
{
	return g->id;
}


uint32_t grille_get_jouee(grille_t *g)
{
	return g->jouee;
}


uint32_t grille_get_renforcee(grille_t *g)
{
	return g->renforcee;
}


uint32_t grille_get_billeSortie(grille_t *g)
{
	return g->billeSortie;
}


uint32_t* grille_get_equiv(grille_t *g)
{
	return g->equiv;
}


uint32_t grille_get_finale(grille_t *g)
{
	return g->finale;
}


/* Accesseurs en écriture */


void grille_set_id(grille_t *g, uint32_t newId)
{
	g->id = newId;
}


void grille_set_jouee(grille_t *g, uint32_t newJouee)
{
	g->jouee = newJouee;
}


void grille_set_renforcee(grille_t *g, uint32_t newRenforcee)
{
	g->renforcee = newRenforcee;
}


void grille_set_billeSortie(grille_t *g, uint32_t newBilleSortie)
{
	g->billeSortie = newBilleSortie;
}


void grille_set_equiv(grille_t *g, uint32_t *newEquiv)
{
	uint32_t i;

	for(i = 0; i < 8; i++)
		g->equiv[i] = newEquiv[i];
}


void grille_set_finale(grille_t *g, uint32_t newFinale)
{
	g->finale = newFinale;
}


/* Manipulation */


void grille_initialiser(grille_t *g)
{
	uint32_t tab[8] = {0}, i;

	for(i = 0; i < 9; i++)
		case_initialiser(&g->cases[i], i+1);

	grille_set_id(g, 0);
	grille_set_jouee(g, VIDE);
	grille_set_renforcee(g, VIDE);
	grille_set_billeSortie(g, 0);
	grille_set_equiv(g, tab);
	grille_set_finale(g, ENCOURS);
}


uint32_t grille_id_calcul(grille_t *g)
{
	uint32_t id = 0, i;

	/* On calcule une première valeur de l'id */
	for(i = 0; i < 9; i++)
	{
		id = id + case_score(&g->cases[i]);
		/* On met à 0 le nbBille des cases remplies pour ne pas tirer leurs numéros au sort */
		if(case_get_coeffEtat(&g->cases[i]) == X || case_get_coeffEtat(&g->cases[i]) == O)
			case_set_nbBille(&g->cases[i], 0);
	}

	/* On s'assure que l'id renvoyé est compris dans les bornes du tableau représentant la mémoire */
	if(id < TAILLEMEMIA)
		return id % TAILLEMEMIA;
	else if(id < 2 * TAILLEMEMIA)
		return (id % TAILLEMEMIA) + 1;
	else
		return (id % TAILLEMEMIA) + 2;
}


uint32_t grille_billeSortie_rot90(uint32_t billeSortie)
{
	switch(billeSortie)
	{
		case 1: return 3;
		case 2: return 6;
		case 3: return 9;
		case 4: return 2;
		case 5: return 5;
		case 6: return 8;
		case 7: return 1;
		case 8: return 4;
		case 9: return 7;
		default: return 0;
	}
}


uint32_t grille_billeSortie_sym(uint32_t billeSortie)
{
	switch(billeSortie)
	{
		case 1: return 3;
		case 2: return 2;
		case 3: return 1;
		case 4: return 6;
		case 5: return 5;
		case 6: return 4;
		case 7: return 9;
		case 8: return 8;
		case 9: return 7;
		default: return 0;
	}
}


grille_t grille_equiv_rot90(grille_t *g)
{
	grille_t newG = *g;
	uint32_t i;

	/* Maj tableau cases : indices de 0 à 8 */
	cases_echanger_etat(newG.cases, 2, 8);
	cases_echanger_etat(newG.cases, 0, 2);
	cases_echanger_etat(newG.cases, 1, 5);
	cases_echanger_etat(newG.cases, 0, 6);
	cases_echanger_etat(newG.cases, 3, 7);
	cases_echanger_etat(newG.cases, 1, 7);

	/* Maj de l'id et de billeSortie */
	newG.id = grille_id_calcul(&newG);
	newG.billeSortie = grille_billeSortie_rot90(g->billeSortie);

	/* Maj tableau equiv */
	for(i = 0; i < 6; i++)
		tab_entier_echanger(newG.equiv, i, i+2);

	return newG;
}


grille_t grille_equiv_sym(grille_t *g)
{
	grille_t newG = *g;
	uint32_t i;

	/* Maj tableau cases : indices de 0 à 8 */
	cases_echanger_etat(newG.cases, 0, 2);
	cases_echanger_etat(newG.cases, 3, 5);
	cases_echanger_etat(newG.cases, 6, 8);

	/* Maj de l'id et de billeSortie */
	newG.id = grille_id_calcul(&newG);
	newG.billeSortie = grille_billeSortie_sym(g->billeSortie);

	/* Maj tableau equiv */
	for(i = 0; i < 8; i = i+2)
		tab_entier_echanger(newG.equiv, i, i+1);

	tab_entier_echanger(newG.equiv, 2, 6);
	tab_entier_echanger(newG.equiv, 3, 7);

	return newG;
}

void grille_equiv_remplir(grille_t *g)
{
	uint32_t tabId[8];
	grille_t rotG, symG;

	/* On remplit le tableau tabId selon la convention choisie (voire rapport) */
	tabId[0] = grille_get_id(g);
	symG = grille_equiv_sym(g);
	tabId[1] = grille_id_calcul(&symG);

	rotG = grille_equiv_rot90(g);
	tabId[2] = grille_id_calcul(&rotG);
	symG = grille_equiv_sym(&rotG);
	tabId[3] = grille_id_calcul(&symG);

	rotG = grille_equiv_rot90(&rotG);
	tabId[4] = grille_id_calcul(&rotG);
	symG = grille_equiv_sym(&rotG);
	tabId[5] = grille_id_calcul(&symG);

	rotG = grille_equiv_rot90(&rotG);
	tabId[6] = grille_id_calcul(&rotG);
	symG = grille_equiv_sym(&rotG);
	tabId[7] = grille_id_calcul(&symG);

	grille_set_equiv(g, tabId);
}

uint32_t grille_test_finale(grille_t *g)
{
	uint32_t i, j;
	uint32_t nbcma;	/* nbcma est le nombre de cases marquées alignées */
	
	/* Détection des 3 alignements X horizontaux */
	for (i = 0 ; i < 3 ; i++)
	{
		nbcma = 0;
		for (j = 0 ; j < 3 ; j++)
		{
			if (case_get_coeffEtat(&g->cases[j+3*i]) == X)
				nbcma++;
			else
				break;
		}
		if(nbcma == 3)
		{
			grille_set_finale(g, XGAGNE);
			return XGAGNE;
		}
	}

	/* Détection des 3 alignements X verticaux */
	for (i = 0 ; i < 3 ; i++)
	{
		nbcma = 0;
		for (j = 0 ; j < 3 ; j++)
		{
			if (case_get_coeffEtat(&g->cases[3*j+i]) == X)
				nbcma++;
			else
				break;
		}
		if(nbcma == 3)
		{
			grille_set_finale(g, XGAGNE);
			return XGAGNE;
		}
	}

	/* Détection des 2 alignements X diagonaux */
	nbcma = 0;
	for (i = 0 ; i < 3 ; i++)
	{
		if (case_get_coeffEtat(&g->cases[i*4]) == X)
			nbcma++;
		else
			break;
	}
	if(nbcma == 3)
	{
		grille_set_finale(g, XGAGNE);
		return XGAGNE;
	}

	nbcma = 0;
	for (i = 1 ; i <= 3 ; i++)
	{
		if (case_get_coeffEtat(&g->cases[i*2]) == X)
			nbcma++;
		else
			break;
	}
	if(nbcma == 3)
	{
		grille_set_finale(g, XGAGNE);
		return XGAGNE;
	}



	/* Détection des 3 alignements O horizontaux */
	for (i = 0 ; i < 3 ; i++)
	{
		nbcma = 0;
		for (j = 0 ; j < 3 ; j++)
		{
			if (case_get_coeffEtat(&g->cases[j+3*i]) == O)
				nbcma++;
			else
				break;
		}
		if(nbcma == 3)
		{
			grille_set_finale(g, OGAGNE);
			return OGAGNE;
		}
	}

	/* Détection des 3 alignements O verticaux */
	for (i = 0 ; i < 3 ; i++)
	{
		nbcma = 0;
		for (j = 0 ; j < 3 ; j++)
		{
			if (case_get_coeffEtat(&g->cases[3*j+i]) == O)
				nbcma++;
			else
				break;
		}
		if(nbcma == 3)
		{
			grille_set_finale(g, OGAGNE);
			return OGAGNE;
		}
	}

	/* Détection des 2 alignements O diagonaux */
	nbcma = 0;
	for (i = 0 ; i < 3 ; i++)
	{
		if (case_get_coeffEtat(&g->cases[i*4]) == O)
			nbcma++;
		else
			break;
	}
	if(nbcma == 3)
	{
		grille_set_finale(g, OGAGNE);
		return OGAGNE;
	}

	nbcma = 0;
	for (i = 1 ; i <= 3 ; i++)
	{
		if (case_get_coeffEtat(&g->cases[i*2]) == O)
			nbcma++;
		else
			break;
	}
	if(nbcma == 3)
	{
		grille_set_finale(g, OGAGNE);
		return OGAGNE;
	}


	/* On vérifie si le tableau est rempli */
	for(i = 0 ; i < 9 && case_get_coeffEtat(&g->cases[i]) != VIDE ; i++)
	{}
	if(i == 9)
	{
		grille_set_finale(g, EGALITE);
		return EGALITE;
	}

	grille_set_finale(g, ENCOURS);
	return ENCOURS;
}
