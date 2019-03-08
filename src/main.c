/* Mohammed EL KHAIRA */
#include "morpion_console.h"


int main()
{
	memoire mem;
	grille_t g;
	grille_t *pg = &g;
	uint32_t choix, nbVicoireO = 0, nbVicoireX = 0, nbEgalite = 0;
	
	srand(time(NULL));

	ia_initialiser(mem);
	grille_initialiser(pg);
	choix = morpion_menu_afficher();

	if(choix == 1)
	{
		morpion_charger_afficher(mem);
		mode1(mem, pg);
	}
	else if(choix == 2)
	{
		morpion_charger_afficher(mem);
		mode2(mem, pg, &nbVicoireO, &nbVicoireX, &nbEgalite);
	}

	return EXIT_SUCCESS;
}
