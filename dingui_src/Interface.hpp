#include "Application.hpp"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

class Interface
{
	Application sonExplorateur;
	int sonX, sonY, sonScroll, sonMenu;
	std::map<std::string, SDL_Surface *> sesIcones;
	SDL_Color sonBlanc, sonNoir, sonFocus;
	SDL_Surface * sonEcran, * sonArrierePlan, * sonFondMessage, * sonFondMenu, * sonIconeDossier, * sonIconeFichier, * sonIconeInconnu;
	TTF_Font * saPolice;
	void afficher();
	void boucler();
	SDL_Surface * chargerImage(std::string) const;
	void ecrire(std::string, int, int, bool);
	SDL_Surface * getSonIcone(std::string) const;
public :
	Interface();
	~Interface();
};

