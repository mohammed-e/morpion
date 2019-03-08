/* Mohammed EL KHAIRA */
#include "case_t.h"



/********************************************************************/
/* Définitions des fonctions de manipulation des cases de la grille */
/********************************************************************/


/* Accesseurs en lecture */


uint32_t case_get_num(case_t *c)
{
	return c->num;
}


uint32_t case_get_coeffEtat(case_t *c)
{
	return c->coeffEtat;
}


uint32_t case_get_nbBille(case_t *c)
{
	return c->nbBille;
}


/* Accesseurs en écriture */


void case_set_num(case_t *c, uint32_t newNum)
{
	c->num = newNum;
}


void case_set_coeffEtat(case_t *c, uint32_t newCoeffEtat)
{
	c->coeffEtat = newCoeffEtat;
}


void case_set_nbBille(case_t *c, uint32_t newNbBille)
{
	c->nbBille = newNbBille;
}


/* Manipulation */


void case_initialiser(case_t *c, uint32_t numCase)
{
	case_set_num(c, numCase);
	case_set_coeffEtat(c, VIDE);
	case_set_nbBille(c, NBBILLEINIT);
}


uint32_t case_score(case_t *c)
{
	return c->coeffEtat * (uint32_t)pow(3, c->num);
}


void tab_entier_echanger(uint32_t *tab, uint32_t i, uint32_t j)
{
	uint32_t tmp;

	/* On échange les entiers contenus par les cases i et j du tableau tab */
	tmp = tab[i];
	tab[i] = tab[j];
	tab[j] = tmp;
}


void cases_echanger_etat(case_t *c, uint32_t i, uint32_t j)
{
	case_t tmp;

	/* On échange seulement le champ coeffEtat des cases i et j */
	tmp.coeffEtat = c[i].coeffEtat;
	c[i].coeffEtat = c[j].coeffEtat;
	c[j].coeffEtat = tmp.coeffEtat;
}




