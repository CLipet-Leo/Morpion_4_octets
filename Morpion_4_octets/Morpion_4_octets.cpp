#include <iostream>
#include <limits>

#define MORPION_GRILLE_MASK   0x3FFFF             // 18 bits pour la grille (9 cases * 2 bits)
#define MORPION_GRILLE_SHIFT  0                   // La grille commence au bit 0
											      
#define MORPION_JOUEUR_MASK   0x40000             // 1 bit pour le joueur en cours
#define MORPION_JOUEUR_SHIFT  18                  // Joueur en cours est au bit 18
											      
#define MORPION_ETAT_MASK     0x180000            // 2 bits pour l'état de la partie
#define MORPION_ETAT_SHIFT    19                  // Etat de la partie est au bit 19
											      
#define MORPION_VICTOIRES_J1_MASK  0xE00000       // 3 bits pour le joueur 1 (0-5 victoires)
#define MORPION_VICTOIRES_J1_SHIFT 21             // Commence au bit 21
											      
#define MORPION_VICTOIRES_J2_MASK  0x7000000      // 3 bits pour le joueur 2 (0-5 victoires)
#define MORPION_VICTOIRES_J2_SHIFT 24             // Commence au bit 24
											      
#define MORPION_NULLS_MASK    0xF8000000          // 5 bits pour les parties nulles (0-20 nulles)
#define MORPION_NULLS_SHIFT   27                  // Commence au bit 27

#define CASE_MASK 0x3                             // 2 bits pour une case

// Fonctions de récupération

int lireCase(unsigned int morpion, int indexCase)
{
	return (morpion >> (MORPION_GRILLE_SHIFT + indexCase * 2)) & CASE_MASK;
}

int actJoueur(unsigned int morpion)
{
	return (morpion >> MORPION_JOUEUR_SHIFT) & 0x1;
}

int actEtat(unsigned int morpion)
{
	return (morpion >> MORPION_ETAT_SHIFT) & 0x3;
}

int actVictoiresJ1(unsigned int morpion)
{
	return (morpion >> MORPION_VICTOIRES_J1_SHIFT) & 0x7;
}

int actVictoiresJ2(unsigned int morpion)
{
	return (morpion >> MORPION_VICTOIRES_J2_SHIFT) & 0x7;
}

int actNulls(unsigned int morpion)
{
	return (morpion >> MORPION_NULLS_SHIFT) & 0x1F;
}

// Fonctions de modification

void majGrille(unsigned int& morpion, int indexCase, int valeur)
{
	morpion &= ~(0x3 << (MORPION_GRILLE_SHIFT + indexCase * 2));
	morpion |= (valeur & 0x3) << (MORPION_GRILLE_SHIFT + indexCase * 2);
}

void majJoueur(unsigned int& morpion)
{
	morpion ^= MORPION_JOUEUR_MASK;
}

void majEtat(unsigned int& morpion, int etat)
{
	morpion &= ~(0x3 << MORPION_ETAT_SHIFT);
	morpion |= (etat & 0x3) << MORPION_ETAT_SHIFT;
}

void reinitialiserGrille(unsigned int& morpion)
{
	morpion &= ~MORPION_GRILLE_MASK;
}

// Fonctions d'affichage

void afficherGrille(unsigned int morpion)
{
	for (int i = 0; i < 9; i++)
	{
		unsigned int valeur = lireCase(morpion, i);
		char symbole = valeur == 0 ? ' ' : valeur == 1 ? 'X' : 'O';
		std::cout << symbole;
		if (i % 3 == 2)
		{
			std::cout << '\n';
			if (i != 8) std::cout << "-+-+-\n";
		}
		else
		{
			std::cout << '|';
		}
	}
	std::cout << std::endl;
}

void afficherStats(unsigned int morpion)
{
	std::cout << "Joueur 1 : " << actVictoiresJ1(morpion) << " victoires" << std::endl;
	std::cout << "Joueur 2 : " << actVictoiresJ2(morpion) << " victoires" << std::endl;
	std::cout << "Parties nulles : " << actNulls(morpion) << std::endl << std::endl;
}

// Fonctions de vérification

int checkColonnes(unsigned int morpion, int victoire)
{
	for (int i = 0; i < 3; i++)
	{
		if (lireCase(morpion, i) != 0 && lireCase(morpion, i) == lireCase(morpion, i + 3) && lireCase(morpion, i) == lireCase(morpion, i + 6))
		{
			return lireCase(morpion, i);
			break;
		}
		if (lireCase(morpion, i * 3) != 0 && lireCase(morpion, i * 3) == lireCase(morpion, i * 3 + 1) && lireCase(morpion, i * 3) == lireCase(morpion, i * 3 + 2))
		{
			return lireCase(morpion, i * 3);
			break;
		}
	}
	return 0;
}

void checkVictory(unsigned int& morpion)
{
	int victoire = 0;

	victoire = checkColonnes(morpion, victoire);
	if (lireCase(morpion, 0) != 0 && lireCase(morpion, 0) == lireCase(morpion, 4) && lireCase(morpion, 0) == lireCase(morpion, 8))
	{
		victoire = lireCase(morpion, 0);
	}
	if (lireCase(morpion, 2) != 0 && lireCase(morpion, 2) == lireCase(morpion, 4) && lireCase(morpion, 2) == lireCase(morpion, 6))
	{
		victoire = lireCase(morpion, 2);
	}

	if (victoire != 0)
	{
		majEtat(morpion, victoire);
		if (victoire == 1)
			morpion += 1 << MORPION_VICTOIRES_J1_SHIFT;
		else
			morpion += 1 << MORPION_VICTOIRES_J2_SHIFT;
		afficherStats(morpion);
		reinitialiserGrille(morpion);
	}
	else
	{
		bool nul = true;
		for (int i = 0; i < 9; i++)
		{
			if (lireCase(morpion, i) == 0)
			{
				nul = false;
				break;
			}
		}
		if (nul)
		{
			majEtat(morpion, 3);
			morpion += 1 << MORPION_NULLS_SHIFT;
			afficherStats(morpion);
			reinitialiserGrille(morpion);
		}
	}
}

// Boucle principale

int main()
{
	unsigned int morpion = 0;

	while (actEtat(morpion) == 0)
	{
		system("cls");
		checkVictory(morpion);
		afficherGrille(morpion);
		std::cout << "Joueur " << (actJoueur(morpion) + 1) << ", entrez un numero de case (0-8) : ";
		int indexCase = 0;
		std::cin >> indexCase; // Remove the static_cast<int> conversion

		if (std::cin.fail() || indexCase < 0 || indexCase > 8 || lireCase(morpion, indexCase) != 0)
		{
			std::cout << "Case invalide !" << std::endl;
			std::cin.clear(); // Clear the error state
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the remaining input
			continue;
		}
		else {
			majGrille(morpion, indexCase, actJoueur(morpion) + 1);
			majJoueur(morpion);
		}
		std::cout << std::endl;
	}

	return 0;
}
