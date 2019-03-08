/* Mohammed EL KHAIRA */
#include "morpion_console.h"




/**************************************************************************/
/* Définitions des fonctions de gestion des parties de morpion en console */
/**************************************************************************/



void morpion_purger_buffer(void)
{
	int c;
	while((c = getchar()) != '\n' && c != EOF);
}


int32_t morpion_tester_marquage(grille_t *g, uint32_t marquage, uint32_t num)
{
	if(num < 1 || num > 9)
	{
		printf("\tLa case %d est incorrecte : il faut choisir le numéro de la case dans [1,%u]\n", num, 9);
		return -1;
	}
	else if(case_get_coeffEtat(&g->cases[num-1]) != VIDE)
	{
		printf("\tLa case %d est deja marquee\n", num);
		return -1;
	}

	/* Marquage de la case */
	case_set_coeffEtat(&g->cases[num-1], marquage);
	
	return 0;
}


uint32_t morpion_menu_afficher(void)
{
	uint32_t choix = -1;

	printf("\n\n- BIENVENU DANS LE JEU DU MORPION -\n\n");

	printf("\n- MENU PRINCIPAL -\n\n");
	printf("1. Joueur VS IA sans entraînement\n");
	printf("2. Joueur VS IA avec entraînement\n");
	printf("3. Quitter\n");

	printf("\nQue souhaitez-vous faire ?\n");
	do
	{
		scanf("%d", &choix);
		morpion_purger_buffer();
	}while(choix < 1 || choix > 3);

	return choix;
}


void morpion_charger_afficher(memoire mem)
{
	FILE *f;
	uint32_t choix = -1;

	f = fopen("save.bin", "rb");
	if(f == NULL)
		printf("\nAucun fichier de sauvegarde n'a été trouvé\n");
	else
	{
		printf("\nUn fichier de sauvegarde a été trouvé\n");
		printf("1. Je veux le charger\n");
		printf("2. Je ne veux pas le charger\n");
		printf("\nQue souhaitez-vous faire ?\n");
		do
		{
			scanf("%d", &choix);
			morpion_purger_buffer();
		}while(choix < 1 || choix > 2);
	}

	if(choix == 1)
		ia_charger(mem, f);

	fclose(f);
}


void morpion_rejouer_afficher(memoire mem, grille_t *g)
{
	uint32_t choix = -1;

	if(grille_get_finale(g) == XGAGNE)
		printf("\nVOUS AVEZ GAGNÉ !\n\n");
	else if(grille_get_finale(g) == OGAGNE)
		printf("\nVOUS AVEZ PERDU !\n\n");
	else if(grille_get_finale(g) == EGALITE)
		printf("\nMATCH NUL !\n\n");
	else
	{
		fprintf(stderr, "\nmorpion_rejouer_afficher : la partie doit être terminée pour l'appeler :\n file \"%s\" line : %d\n", __FILE__, __LINE__);	
		exit(EXIT_FAILURE);
	}

	printf("1. Rejouer\n");
	printf("2. Quitter\n");
	printf("\nQue souhaitez-vous faire ?\n");
	do
	{
		scanf("%d", &choix);
		morpion_purger_buffer();
	}while(choix < 1 || choix > 2);

	if(choix == 1)
		mode1(mem, g);
	else
		ia_sauvegarder(mem);
}


void morpion_grille_afficher(grille_t *g)
{
	uint32_t i, j;

	/* Grille de jeu */	
	for(i=1 ; i <= 9 ; i++)
	{
		/* Récupération du coeffEtat sur la case i et affichage adapté */
		switch(case_get_coeffEtat(&g->cases[i-1]))
		{
			case VIDE:
				printf("   ");
				break;
			case X:
				printf(" X ");
				break;
			case O:
				printf(" O ");
				break;
			default:
				printf("   ");
				break;
		}
		if(i%3 == 0)
		{
			printf("\n");
			for(j=0 ; j < 3 ; j++)
				printf("---");
			printf("--\n");
		}
		else
			printf("|");
	}
}


void morpion_afficher_jeu(grille_t* g)
{
	uint32_t etat = grille_get_finale(g);

	/* Etat de la partie */
	if(etat == XGAGNE)
		printf("\nCROIX a GAGNE!\n");
	else if(etat == OGAGNE)
		printf("\nCERCLE a GAGNE!\n");
	else if(etat == EGALITE)
		printf("\nEGALITE!\n");
	else if(etat == ENCOURS)
		printf("\nPARTIE EN COURS\n");
	else
	{
		fprintf(stderr, "\nmorpion_afficher_jeu : la partie est dans un etat indefinie :\n file \"%s\" line : %d\n", __FILE__, __LINE__);	
		exit(EXIT_FAILURE);
	}

	/* Grille de jeu */	
	morpion_grille_afficher(g);
}


void morpion_humain_jouer(grille_t *g)
{
	uint32_t num = -1;

	printf("\nCROIX JOUE\n");
	
	do
	{
		scanf("%d", &num);
		morpion_purger_buffer();
	}while(morpion_tester_marquage(g, X, num) == -1); /* l'humain joue toujours les X*/

	morpion_afficher_jeu(g);
}

void morpion_ia_jouer(memoire mem, grille_t *g, uint32_t marquage, uint32_t mode)
{
	uint32_t iaCaseTiree;

	/* On ajoute la grille en mémoire si ce n'est pas déjà fait */
	/* Les champs id, equiv et finale sont maj avant l'ajout de la grille */
	grille_set_id(g, grille_id_calcul(g));
	if(ia_id_rechercher(mem, g) == NULL)
	{
		grille_equiv_remplir(g);
		ia_grille_ajouter(mem, g);
	}

	/* On détermine le prochain coup */
	/* Les champs jouee, renforcee et billeSortie sont maj avec ia_tirage_billeSortie */
	iaCaseTiree = ia_tirage_billeSortie(mem, g, marquage);
	if(iaCaseTiree == 0)
		return ;

	if(morpion_tester_marquage(g, marquage, iaCaseTiree) == -1)
	{
		fprintf(stderr, "\nmorpion_ia_jouer : ne doit pas jouer sur une case remplie :\n file \"%s\" line : %d\n", __FILE__, __LINE__);	
		exit(EXIT_FAILURE);
	}

	if(mode == 1)
	{
		printf("\nROND JOUE\n");
		morpion_afficher_jeu(g);
	}
}

void morpion_ia_jouer_alea(grille_t *g, uint32_t marquage)
{
	uint32_t tab[9] = {0}; /* tab sera remplis avec les num des cases vides */
	int i, j; /* i permet de parcourir le tableau g->cases */

	for(i = 0, j = 0 ; i < 9 ; i++)
		if(case_get_coeffEtat(&g->cases[i]) == VIDE)
			tab[j++] = i+1; /* on remplis tab avec le num des cases vides */

	if(0 == j)
	{
		fprintf(stderr, "\nia_jouer_alea : erreur, il n'y a plus de cases vide :\n file \"%s\" line : %d\n", __FILE__, __LINE__);	
		exit(EXIT_FAILURE);
	}
	morpion_tester_marquage(g, marquage, tab[rand()%j]); /* on marque une cases au hasard parmis les cases vides (rand()%j est dans [0;8])*/
}


void mode1(memoire mem, grille_t *g)
{
	uint32_t aleaPremier = (rand() % 2) + 1;

	/* On s'assure que la grille est vide pour commencer la partie */
	grille_initialiser(g);
	morpion_afficher_jeu(g);

	do
	{
		if(aleaPremier == 1)
		{
			morpion_humain_jouer(g);
			if(grille_test_finale(g) != ENCOURS)
				break;

			morpion_ia_jouer(mem, g, O, 1);
		}
		else if(aleaPremier == 2)
		{
			morpion_ia_jouer(mem, g, O, 1);
			if(grille_test_finale(g) != ENCOURS)
				break;

			morpion_humain_jouer(g);
		}
		else
		{
			fprintf(stderr, "\nmode1 : erreur dans le choix de celui qui commence :\n file \"%s\" line : %d\n", __FILE__, __LINE__);	
			exit(EXIT_FAILURE);
		}
	}while(grille_test_finale(g) == ENCOURS);

	/* ia_renforcement() prend en charge la modification des champs jouee et renforcee */
	ia_renforcement(mem, grille_get_finale(g), O);	

	morpion_rejouer_afficher(mem, g);
}


void mode2(memoire mem, grille_t *g, uint32_t *nbVicoireO, uint32_t *nbVicoireX, uint32_t *nbEgalite)
{
	int32_t N = -1, nbTot = 0;
	uint32_t aleaPremier = (rand() % 2) + 1;

	/* On s'assure que les nb reviennent à 0 */
	*nbVicoireO = 0; *nbVicoireX = 0; *nbEgalite = 0;

	printf("Combien de parties l'IA doit-elle jouer seule ? (entrez un entier supérieur à 0)\n");
	do
	{
		scanf("%d", &N);
		morpion_purger_buffer();
	}while(N <= 0);

	/* N parties sont jouées */
	do
	{
		grille_initialiser(g);

		/* Une partie est jouée */
		do
		{
			if(aleaPremier == 1)
			{
				morpion_ia_jouer(mem, g, O, 2);	/* notre IA (O) */
				if(grille_test_finale(g) != ENCOURS)
					break;
				/* tests pas vraiment concluant */
				/*if(nbTot % 2 == 0)
					morpion_ia_jouer_alea(g, X);
				else*/
				morpion_ia_jouer(mem, g, X, 2);	/* son jumeau maléfique (X) */
			}
			else if(aleaPremier == 2)
			{
				/* tests pas vraiment concluant */
				/*if(nbTot % 2 == 0)
					morpion_ia_jouer_alea(g, X);
				else*/
				morpion_ia_jouer(mem, g, X, 2);
				if(grille_test_finale(g) != ENCOURS)
					break;

				morpion_ia_jouer(mem, g, O, 2);
			}
			else
			{
				fprintf(stderr, "\nmode2 : erreur dans le choix de celui qui commence :\n file \"%s\" line : %d\n", __FILE__, __LINE__);	
				exit(EXIT_FAILURE);
			}
		}while(grille_test_finale(g) == ENCOURS);


		/* Statistiques */
		if(grille_get_finale(g) == OGAGNE)
		{
			(*nbVicoireO)++;
			nbTot++;
		}
		else if(grille_get_finale(g) == XGAGNE)
		{
			(*nbVicoireX)++;
			nbTot++;
		}
		else if(grille_get_finale(g) == EGALITE)
		{
			(*nbEgalite)++;
			nbTot++;
		}

		/* ia_renforcement() prend en charge la modification des champs jouee et renforcee */
		ia_renforcement(mem, grille_get_finale(g), O);
		ia_renforcement(mem, grille_get_finale(g), X);

	}while(--N > 0);

	printf("L'IA a obtenue %d%% victoires, %d%% défaites et %d%% matchs nuls\n\n", *nbVicoireO*100/nbTot, *nbVicoireX*100/nbTot, *nbEgalite*100/nbTot);

	/* On revient au mode humain VS IA */
	mode1(mem, g);
}








