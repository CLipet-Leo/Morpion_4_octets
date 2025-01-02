#include <iostream>

#define MORPION_GRILLE_MASK   0x3FFFF       // 18 bits pour la grille (9 cases * 2 bits)
#define MORPION_GRILLE_SHIFT  0             // La grille commence au bit 0

#define MORPION_JOUEUR_MASK   0x40000       // 1 bit pour le joueur en cours
#define MORPION_JOUEUR_SHIFT  18            // Joueur en cours est au bit 18

#define MORPION_ETAT_MASK     0x180000      // 2 bits pour l'état de la partie
#define MORPION_ETAT_SHIFT    19            // Etat de la partie est au bit 19

#define MORPION_VICTOIRES_J1_MASK  0xE00000  // 3 bits pour le joueur 1 (0-5 victoires)
#define MORPION_VICTOIRES_J1_SHIFT 21        // Commence au bit 21

#define MORPION_VICTOIRES_J2_MASK  0x7000000 // 3 bits pour le joueur 2 (0-5 victoires)
#define MORPION_VICTOIRES_J2_SHIFT 24        // Commence au bit 24

#define MORPION_NULLS_MASK    0xF8000000   // 5 bits pour les parties nulles (0-20 nulles)
#define MORPION_NULLS_SHIFT   27           // Commence au bit 27

#define CASE_MASK 0x3
#define CASE_SHIFT(i) (i * 2)

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

void afficherGrille(unsigned int morpion)
{
	for (int i = 0; i < 9; i++)
	{
		unsigned int valeur = lireCase(morpion, i);
		char symbole = valeur == 0 ? ' ' : valeur == 1 ? 'X' : 'O';
		std::cout << symbole << (i % 3 == 2 ? '\n' : '|');
	}
}

#include <iostream>
#include <limits> // Add this include statement

// ...

int main()
{
	unsigned int morpion = 0;

	while (actEtat(morpion) == 0)
	{
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
