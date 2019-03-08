/* Mohammed EL KHAIRA */
#include "ia.h"



/*******************************************************************/
/* Définitions des fonctions de manipulation de la mémoire de l'IA */
/*******************************************************************/


void ia_initialiser(memoire mem)
{
	uint32_t i;

	for(i = 0; i < TAILLEMEMIA; i++)
		grille_initialiser(&mem[i]);
}


uint32_t ia_id_existe(memoire mem, grille_t* g)
{
	return mem[grille_get_id(g)].id == grille_get_id(g);
}


grille_t* ia_id_rechercher(memoire mem, grille_t* g)
{
	if(ia_id_existe(mem, g))
		return &mem[grille_get_id(g)];
	return NULL;
}


void ia_grille_ajouter(memoire mem, grille_t* g)
{
	if(ia_id_rechercher(mem, g) == NULL)
	{
		uint32_t i;

		for(i = 0; i < 9; i++)
		{
			case_set_num(&mem[grille_get_id(g)].cases[i], g->cases[i].num);
			case_set_coeffEtat(&mem[grille_get_id(g)].cases[i], g->cases[i].coeffEtat);
			case_set_nbBille(&mem[grille_get_id(g)].cases[i], g->cases[i].nbBille);
		}

		grille_set_id(&mem[grille_get_id(g)], grille_get_id(g));
		grille_set_jouee(&mem[grille_get_id(g)], grille_get_jouee(g));
		grille_set_billeSortie(&mem[grille_get_id(g)], grille_get_billeSortie(g));
		grille_set_equiv(&mem[grille_get_id(g)], grille_get_equiv(g));
		grille_set_finale(&mem[grille_get_id(g)], grille_get_finale(g));
	}
	else
		printf("ia_grille_ajouter : la grille ne doit pas déjà exister en mémoire pour l'appeler\n");
}


uint32_t ia_tirage_billeSortie(memoire mem, grille_t *g, uint32_t marquage)
{
	uint32_t caseTiree = 0, poidTotal = mem[grille_get_id(g)].cases[0].nbBille, i;
	uint32_t indices[9] = {0}; indices[0] = mem[grille_get_id(g)].cases[0].nbBille;

	/* Maj des champs jouee et renforcee (l'autre maj a lieu dans ia_renforcement()) */
	grille_set_jouee(&mem[grille_get_id(g)], marquage);
	grille_set_renforcee(&mem[grille_get_id(g)], VIDE);
	grille_set_renforcee(&mem[mem[grille_get_id(g)].equiv[1]], VIDE);
	grille_set_renforcee(&mem[mem[grille_get_id(g)].equiv[2]], VIDE);
	grille_set_renforcee(&mem[mem[grille_get_id(g)].equiv[3]], VIDE);
	grille_set_renforcee(&mem[mem[grille_get_id(g)].equiv[4]], VIDE);
	grille_set_renforcee(&mem[mem[grille_get_id(g)].equiv[5]], VIDE);
	grille_set_renforcee(&mem[mem[grille_get_id(g)].equiv[6]], VIDE);
	grille_set_renforcee(&mem[mem[grille_get_id(g)].equiv[7]], VIDE);

	/* Calcul des variables dont on a besoin pour le tirage avec poids */
	for(i = 1; i < 9; i++)
	{
		poidTotal = poidTotal + mem[grille_get_id(g)].cases[i].nbBille;
		indices[i] = indices[i-1] + mem[grille_get_id(g)].cases[i].nbBille;
	}

	/* Si il n'y a plus de bille dans toutes les cases, la partie est terminée avec IA perdante */
	if(poidTotal == 0)
	{
		grille_set_finale(g, XGAGNE);
		/* Si c'est la grille de départ alors l'apprentissage reprend du début */
		if(grille_get_id(&mem[grille_get_id(g)]) == 0)
			ia_initialiser(mem);
		/* On réinitialise le nbBille à NBBILLEINIT (pour poursuivre dans le cas ou ce n'est pas la grille de départ) */
		for(i = 0; i < 9; i++)
			if(case_get_coeffEtat(&mem[grille_get_id(g)].cases[i]) == VIDE)
				case_set_nbBille(&mem[grille_get_id(g)].cases[i], NBBILLEINIT);

		printf("Il n'y a plus de bille sur la grille %d : ce nombre a été réinitialisé pour la suite\n", grille_get_id(g));
		/* Le renvoie de 0 permet d'identifier le cas où il n'y a plus de bille */
		return 0;
	}
	/* Sinon le tirage aléatoire avec poids a lieu, avec maj du champ billeSortie */
	/* Avec ce tirage, mem[grille_get_id(g)].cases[i].num est renvoyé avec une proba de (mem[grille_get_id(g)].cases[i].nbBille / poidTotal) */
	else
	{
		caseTiree = (rand() % (poidTotal)) + 1; /* caseTiree entre 1 et poidTotal */

		if(caseTiree <= indices[0] && caseTiree >= 1)
		{
			grille_set_billeSortie(&mem[grille_get_id(g)], mem[grille_get_id(g)].cases[0].num);
			return mem[grille_get_id(g)].cases[0].num;
		}
		else
		{
			for(i = 1; i < 9; i++)
				if(caseTiree <= indices[i] && caseTiree > indices[i-1])
				{
					grille_set_billeSortie(&mem[grille_get_id(g)], mem[grille_get_id(g)].cases[i].num);
					return mem[grille_get_id(g)].cases[i].num;
				}
		}
	}

	/* Contrôle d'erreurs */
	fprintf(stderr, "\nia_tirage_billeSortie() : doit renvoyer un entier :\n file \"%s\" line : %d\n", __FILE__, __LINE__);	
	exit(EXIT_FAILURE);
}


void ia_renforcement(memoire mem, uint32_t resultat, uint32_t marquage)
{
	uint32_t i;

	if(marquage == O)
	{
		for(i = 0; i < TAILLEMEMIA; i++)
		{
			if(mem[i].jouee == marquage)
			{
				if(resultat == OGAGNE)
				{
					if(grille_get_renforcee(&mem[i]) == VIDE)
					{
						mem[i].cases[mem[i].billeSortie-1].nbBille += 3;
						grille_set_renforcee(&mem[i], O);
					}

					/* Renforcement des grilles équivalentes */
					if(grille_get_renforcee(&mem[mem[i].equiv[1]]) == VIDE)
					{
						mem[mem[i].equiv[1]].cases[grille_billeSortie_sym(mem[i].billeSortie)-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[1]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[2]]) == VIDE)
					{
						mem[mem[i].equiv[2]].cases[grille_billeSortie_rot90(mem[i].billeSortie)-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[2]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[3]]) == VIDE)
					{
						mem[mem[i].equiv[3]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(mem[i].billeSortie))-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[3]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[4]]) == VIDE)
					{
						mem[mem[i].equiv[4]].cases[grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie))-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[4]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[5]]) == VIDE)
					{
						mem[mem[i].equiv[5]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie)))-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[5]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[6]]) == VIDE)
					{
						mem[mem[i].equiv[6]].cases[grille_billeSortie_rot90(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie)))-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[6]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[7]]) == VIDE)
					{
						mem[mem[i].equiv[7]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie))))-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[7]], O);
					}

				}

				else if(resultat == EGALITE)
				{
					if(grille_get_renforcee(&mem[i]) == VIDE)
					{
						mem[i].cases[mem[i].billeSortie-1].nbBille++;
						grille_set_renforcee(&mem[i], O);
					}

					/* Renforcement des grilles équivalentes */
					if(grille_get_renforcee(&mem[mem[i].equiv[1]]) == VIDE)
					{
						mem[mem[i].equiv[1]].cases[grille_billeSortie_sym(mem[i].billeSortie)-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[1]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[2]]) == VIDE)
					{
						mem[mem[i].equiv[2]].cases[grille_billeSortie_rot90(mem[i].billeSortie)-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[2]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[3]]) == VIDE)
					{
						mem[mem[i].equiv[3]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(mem[i].billeSortie))-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[3]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[4]]) == VIDE)
					{
						mem[mem[i].equiv[4]].cases[grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie))-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[4]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[5]]) == VIDE)
					{
						mem[mem[i].equiv[5]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie)))-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[5]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[6]]) == VIDE)
					{
						mem[mem[i].equiv[6]].cases[grille_billeSortie_rot90(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie)))-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[6]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[7]]) == VIDE)
					{
						mem[mem[i].equiv[7]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie))))-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[7]], O);
					}

				}

				else if(resultat == XGAGNE)
				{
					if(grille_get_renforcee(&mem[i]) == VIDE)
					{
						mem[i].cases[mem[i].billeSortie-1].nbBille--;
						grille_set_renforcee(&mem[i], O);
					}

					/* Renforcement des grilles équivalentes */
					if(grille_get_renforcee(&mem[mem[i].equiv[1]]) == VIDE)
					{
						mem[mem[i].equiv[1]].cases[grille_billeSortie_sym(mem[i].billeSortie)-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[1]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[2]]) == VIDE)
					{
						mem[mem[i].equiv[2]].cases[grille_billeSortie_rot90(mem[i].billeSortie)-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[2]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[3]]) == VIDE)
					{
						mem[mem[i].equiv[3]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(mem[i].billeSortie))-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[3]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[4]]) == VIDE)
					{
						mem[mem[i].equiv[4]].cases[grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie))-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[4]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[5]]) == VIDE)
					{
						mem[mem[i].equiv[5]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie)))-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[5]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[6]]) == VIDE)
					{
						mem[mem[i].equiv[6]].cases[grille_billeSortie_rot90(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie)))-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[6]], O);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[7]]) == VIDE)
					{
						mem[mem[i].equiv[7]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie))))-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[7]], O);
					}

				}

				else
				{
					fprintf(stderr, "\nia_renforcement() : la partie doit être terminée pour l'appeler :\n file \"%s\" line : %d\n", __FILE__, __LINE__);	
					exit(EXIT_FAILURE);
				}
			}

			grille_set_jouee(&mem[i], VIDE);
		}
	}

	else if(marquage == X)
	{
		for(i = 0; i < TAILLEMEMIA; i++)
		{
			if(mem[i].jouee == marquage)
			{
				if(resultat == XGAGNE)
				{
					if(grille_get_renforcee(&mem[i]) == VIDE)
					{
						mem[i].cases[mem[i].billeSortie-1].nbBille += 3;
						grille_set_renforcee(&mem[i], X);
					}

					/* Renforcement des grilles équivalentes */
					if(grille_get_renforcee(&mem[mem[i].equiv[1]]) == VIDE)
					{
						mem[mem[i].equiv[1]].cases[grille_billeSortie_sym(mem[i].billeSortie)-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[1]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[2]]) == VIDE)
					{
						mem[mem[i].equiv[2]].cases[grille_billeSortie_rot90(mem[i].billeSortie)-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[2]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[3]]) == VIDE)
					{
						mem[mem[i].equiv[3]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(mem[i].billeSortie))-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[3]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[4]]) == VIDE)
					{
						mem[mem[i].equiv[4]].cases[grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie))-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[4]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[5]]) == VIDE)
					{
						mem[mem[i].equiv[5]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie)))-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[5]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[6]]) == VIDE)
					{
						mem[mem[i].equiv[6]].cases[grille_billeSortie_rot90(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie)))-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[6]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[7]]) == VIDE)
					{
						mem[mem[i].equiv[7]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie))))-1].nbBille += 3;
						grille_set_renforcee(&mem[mem[i].equiv[7]], X);
					}

				}

				else if(resultat == EGALITE)
				{
					if(grille_get_renforcee(&mem[i]) == VIDE)
					{
						mem[i].cases[mem[i].billeSortie-1].nbBille++;
						grille_set_renforcee(&mem[i], X);
					}

					/* Renforcement des grilles équivalentes */
					if(grille_get_renforcee(&mem[mem[i].equiv[1]]) == VIDE)
					{
						mem[mem[i].equiv[1]].cases[grille_billeSortie_sym(mem[i].billeSortie)-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[1]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[2]]) == VIDE)
					{
						mem[mem[i].equiv[2]].cases[grille_billeSortie_rot90(mem[i].billeSortie)-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[2]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[3]]) == VIDE)
					{
						mem[mem[i].equiv[3]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(mem[i].billeSortie))-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[3]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[4]]) == VIDE)
					{
						mem[mem[i].equiv[4]].cases[grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie))-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[4]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[5]]) == VIDE)
					{
						mem[mem[i].equiv[5]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie)))-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[5]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[6]]) == VIDE)
					{
						mem[mem[i].equiv[6]].cases[grille_billeSortie_rot90(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie)))-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[6]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[7]]) == VIDE)
					{
						mem[mem[i].equiv[7]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie))))-1].nbBille++;
						grille_set_renforcee(&mem[mem[i].equiv[7]], X);
					}

				}

				else if(resultat == OGAGNE)
				{
					if(grille_get_renforcee(&mem[i]) == VIDE)
					{
						mem[i].cases[mem[i].billeSortie-1].nbBille--;
						grille_set_renforcee(&mem[i], X);
					}

					/* Renforcement des grilles équivalentes */
					if(grille_get_renforcee(&mem[mem[i].equiv[1]]) == VIDE)
					{
						mem[mem[i].equiv[1]].cases[grille_billeSortie_sym(mem[i].billeSortie)-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[1]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[2]]) == VIDE)
					{
						mem[mem[i].equiv[2]].cases[grille_billeSortie_rot90(mem[i].billeSortie)-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[2]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[3]]) == VIDE)
					{
						mem[mem[i].equiv[3]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(mem[i].billeSortie))-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[3]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[4]]) == VIDE)
					{
						mem[mem[i].equiv[4]].cases[grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie))-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[4]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[5]]) == VIDE)
					{
						mem[mem[i].equiv[5]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie)))-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[5]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[6]]) == VIDE)
					{
						mem[mem[i].equiv[6]].cases[grille_billeSortie_rot90(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie)))-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[6]], X);
					}
					if(grille_get_renforcee(&mem[mem[i].equiv[7]]) == VIDE)
					{
						mem[mem[i].equiv[7]].cases[grille_billeSortie_sym(grille_billeSortie_rot90(grille_billeSortie_rot90(grille_billeSortie_rot90(mem[i].billeSortie))))-1].nbBille--;
						grille_set_renforcee(&mem[mem[i].equiv[7]], X);
					}

				}

				else
				{
					fprintf(stderr, "\nia_renforcement() : la partie doit être terminée pour l'appeler :\n file \"%s\" line : %d\n", __FILE__, __LINE__);	
					exit(EXIT_FAILURE);
				}
			}

			grille_set_jouee(&mem[i], VIDE);
		}
	}

	else
	{
		fprintf(stderr, "\nia_renforcement : marquage doit valoir X ou O :\n file \"%s\" line : %d\n", __FILE__, __LINE__);	
		exit(EXIT_FAILURE);
	}

}


void ia_sauvegarder(memoire mem)
{
	FILE *f;

	/* Ouverture en mode ecriture pour créer le fichier */
	if(NULL == (f = fopen("save.bin", "wb"))) 		
	{
		fprintf(stderr, "ia_sauvegarder : erreur d'ouverture de fichier :\nfile \"%s\" line : %d\n", __FILE__, __LINE__);	
		exit(EXIT_FAILURE);
	}

	fwrite(mem, sizeof(memoire), 1, f);
	printf("Sauvegarde de l'IA effectué\n");

	fclose(f);
}


void ia_charger(memoire mem, FILE *save)
{
	fread(mem, sizeof(memoire), 1, save);
	printf("Chargement de l'IA effectué\n");
}









