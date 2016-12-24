#include "Interface.hpp"
#include <iostream>
#include <fstream>
#include <SDL/SDL_image.h>
#include <sstream>

Interface::Interface()
:sonX(0)
,sonY(0)
,sonScroll(0)
,sonMenu(0)
,sonEcran(NULL)
,sonArrierePlan(NULL)
,sonFondMessage(NULL)
,sonFondMenu(NULL)
,sonIconeDossier(NULL)
,sonIconeFichier(NULL)
,sonIconeInconnu(NULL)
,saPolice(NULL)
{

	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		std::cerr << "SDL_Init(): " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	sonEcran = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE);

	if (sonEcran == NULL)
	{
		std::cerr << "SDL_SetVideoMode(): " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	SDL_EnableKeyRepeat(500, 100);
	SDL_ShowCursor(SDL_DISABLE);
	atexit(SDL_Quit);

	if (TTF_Init() == -1)
	{
		std::cerr << "TTF_Init(): " << TTF_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	saPolice = TTF_OpenFont(sonExplorateur.getSaValeur("font-family").c_str(), atoi(sonExplorateur.getSaValeur("font-size").c_str()));

	if (saPolice == NULL)
	{
		std::cerr << "TTF_OpenFont(): " << TTF_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	sonBlanc.r = 255;
	sonBlanc.g = 255;
	sonBlanc.b = 255;
	sonNoir.r = 0;
	sonNoir.g = 0;
	sonNoir.b = 0;
	sonFocus.r = atoi(sonExplorateur.getSaValeur("focus-r").c_str());
	sonFocus.g = atoi(sonExplorateur.getSaValeur("focus-g").c_str());
	sonFocus.b = atoi(sonExplorateur.getSaValeur("focus-b").c_str());
	std::ifstream lesIcones("icons.txt");
	std::string lExtension, lIcone;

	while (lesIcones.good() == true)
	{
		getline(lesIcones, lExtension, '\t');
		getline(lesIcones, lIcone, '\n');

		if (lExtension == "" || lIcone == "" || sesIcones.insert(std::make_pair(lExtension, chargerImage(lIcone))).second == false)
		{
			sonExplorateur.ajouterErreur("The icons.txt file is corrupted");
		}

	}

	lesIcones.close();
	sonArrierePlan = chargerImage(sonExplorateur.getSaValeur("background-image"));
	sonFondMessage = chargerImage(sonExplorateur.getSaValeur("foreground-image"));
	sonFondMenu = chargerImage(sonExplorateur.getSaValeur("menu-image"));
	sonIconeDossier = chargerImage(sonExplorateur.getSaValeur("folder-icon"));
	sonIconeFichier = chargerImage(sonExplorateur.getSaValeur("file-icon"));
	sonIconeInconnu = chargerImage(sonExplorateur.getSaValeur("unknown-icon"));
	sonExplorateur.lireRepertoire();
	afficher();
	boucler();
}

Interface::~Interface()
{
	TTF_CloseFont(saPolice);
	TTF_Quit();
	SDL_FreeSurface(sonArrierePlan);
	SDL_FreeSurface(sonFondMessage);
	SDL_FreeSurface(sonFondMenu);
	SDL_FreeSurface(sonIconeDossier);
	SDL_FreeSurface(sonIconeFichier);
	SDL_FreeSurface(sonIconeInconnu);
	SDL_Quit();
}

void Interface::boucler()
{
	int nb_icons_x = atoi(sonExplorateur.getSaValeur("nb-icons-x").c_str());
	int nb_icons_y = atoi(sonExplorateur.getSaValeur("nb-icons-y").c_str());
	std::vector<std::vector<Element *> > leContenu = sonExplorateur.getSonContenu();
	bool b = true;
	SDL_Event lEvenement;

	while (b)
	{

		if (SDL_WaitEvent(&lEvenement))
		{

			if (lEvenement.type == SDL_KEYDOWN)
			{

				if (sonExplorateur.getSesErreurs() != "")
				{

					if (lEvenement.key.keysym.sym == SDLK_LCTRL)
					{
						sonExplorateur.resetSesErreurs();
					}

				}

				else
				{

					if (sonMenu != 0)
					{

						switch (lEvenement.key.keysym.sym)
						{
							case SDLK_UP :
								switch (sonMenu)
								{
									case 9 :
										sonMenu = 5;
										break;
									case 1 :
										sonMenu = 10;
										break;
									default :
										sonMenu--;
								}
								break;
							case SDLK_DOWN :
								switch (sonMenu)
								{
									case 5 :
										sonMenu = 9;
										break;
									case 10 :
										sonMenu = 1;
										break;
									default :
										sonMenu++;
								}
								break;
							case SDLK_LCTRL :
								switch (sonMenu)
								{
									case 1 :
										switch (leContenu[sonY][sonX]->getSonType())
										{
											case dossier :
												sonExplorateur.changerRepertoire(leContenu[sonY][sonX]->getSonNom());
												sonScroll = 0;
												sonX = 0;
												sonY = 0;
												sonExplorateur.lireRepertoire();
												leContenu = sonExplorateur.getSonContenu();
												break;
											case fichier :
												sonExplorateur.ouvrirFichier(leContenu[sonY][sonX]);
												break;
											default :
												sonExplorateur.ajouterErreur("Could not open the file.");
										}
										sonMenu = 0;
										break;
									case 2 :
										sonExplorateur.copier(leContenu[sonY][sonX]->getSonNom());
										sonMenu = 0;
										break;
									case 3 :
										sonExplorateur.coller("");
										sonMenu = 0;
										sonExplorateur.lireRepertoire();
										leContenu = sonExplorateur.getSonContenu();
										break;
									case 4 :
										if (leContenu[sonY][sonX]->getSonType() == dossier)
										{
											sonExplorateur.coller(leContenu[sonY][sonX]->getSonNom());
											sonMenu = 0;
											sonExplorateur.lireRepertoire();
											leContenu = sonExplorateur.getSonContenu();
										}
										break;
									case 5 :
										sonExplorateur.supprimer(leContenu[sonY][sonX]->getSonNom());
										sonMenu = 0;
										sonScroll = 0;
										sonX = 0;
										sonY = 0;
										sonExplorateur.lireRepertoire();
										leContenu = sonExplorateur.getSonContenu();
										break;
									case 9 :
										sonExplorateur.eteindre();
										sonMenu = 0;
										break;
									case 10 :
										sonExplorateur.redemarrer();
										sonMenu = 0;
										break;
									default :
										sonMenu = 0;
								}
								break;
							case SDLK_LALT :
								sonMenu = 0;
								break;
							case SDLK_LSHIFT :
								sonMenu = 0;
								break;
						}

					}

					else
					{

						switch (lEvenement.key.keysym.sym)
						{
							case SDLK_UP :
								if (sonY != 0)
								{
									sonY--;
									if (sonY < sonScroll)
									{
										sonScroll--;
									}
								}
								break;
							case SDLK_DOWN :
								if (leContenu[sonY + 1][sonX] != NULL)
								{
									sonY++;
									if (sonY >= sonScroll + nb_icons_y)
									{
										sonScroll++;
									}
								}
								break;
							case SDLK_LEFT :
								if (sonX != 0)
								{
									sonX--;
								}
								break;
							case SDLK_RIGHT :
								if (sonX + 1 < nb_icons_x && leContenu[sonY][sonX + 1] != NULL)
								{
									sonX++;
								}
								break;
							case SDLK_LCTRL :
								switch (leContenu[sonY][sonX]->getSonType())
								{
									case dossier :
										sonExplorateur.changerRepertoire(leContenu[sonY][sonX]->getSonNom());
										sonScroll = 0;
										sonX = 0;
										sonY = 0;
										sonExplorateur.lireRepertoire();
										leContenu = sonExplorateur.getSonContenu();
										break;
									case fichier :
										sonExplorateur.ouvrirFichier(leContenu[sonY][sonX]);
										break;
									default :
										sonExplorateur.ajouterErreur("Could not open the file.");
								}
								break;
							case SDLK_LALT :
								sonExplorateur.changerRepertoire("..");
								sonScroll = 0;
								sonX = 0;
								sonY = 0;
								sonExplorateur.lireRepertoire();
								leContenu = sonExplorateur.getSonContenu();
								break;
							case SDLK_LSHIFT :
								sonMenu = 1;
								break;
							case SDLK_ESCAPE :
								b = false;
								break;
						}

					}

				}

				afficher();
			}

		}

	}

	return;
}

void Interface::afficher()
{

	if (SDL_BlitSurface(sonArrierePlan, NULL, sonEcran, NULL) == -1)
	{
		std::cerr << "SDL_BlitSurface(): " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	int leX = 0;
	int leY = 0;
	SDL_Rect laPosition;
	std::string leNom;
	int nb_icons_x = atoi(sonExplorateur.getSaValeur("nb-icons-x").c_str());
	int nb_icons_y = atoi(sonExplorateur.getSaValeur("nb-icons-y").c_str());
	int icon_width = atoi(sonExplorateur.getSaValeur("icon-width").c_str());
	int icon_height = atoi(sonExplorateur.getSaValeur("icon-height").c_str());
	int font_size = atoi(sonExplorateur.getSaValeur("font-size").c_str());
	unsigned int name_max_size = ((320 / nb_icons_x) / (font_size / 2 )) - 1;
	std::vector<std::vector<Element *> > leContenu = sonExplorateur.getSonContenu();
	std::vector<std::vector<Element *> >::const_iterator leItY;
	std::vector<Element *>::const_iterator leItX;

	for (leItY = leContenu.begin(); leItY != leContenu.end(); leItY++)
	{

		for (leItX = leItY->begin(); leItX != leItY->end(); leItX++)
		{

			if ((*leItX) != NULL && leY >= sonScroll && leY < sonScroll + nb_icons_y)
			{
				laPosition.x = leX * (320 / nb_icons_x) + (320 / nb_icons_x - icon_width) / 2;
				laPosition.y = (leY - sonScroll) * (240 / nb_icons_y) + (240 / nb_icons_y - icon_height) / 2;

				switch ((*leItX)->getSonType())
				{
					case dossier :
						if (SDL_BlitSurface(sonIconeDossier, NULL, sonEcran, &laPosition) == -1)
						{
							std::cerr << "SDL_BlitSurface(): " << SDL_GetError() << std::endl;
							exit(EXIT_FAILURE);
						}
						break;
					case fichier :
					{
						SDL_Surface * lIcone = getSonIcone((*leItX)->getSonExtension());
						if (lIcone == NULL)
						{
							lIcone = sonIconeFichier;
						}
						if (SDL_BlitSurface(lIcone, NULL, sonEcran, &laPosition) == -1)
						{
							std::cerr << "SDL_BlitSurface(): " << SDL_GetError() << std::endl;
							exit(EXIT_FAILURE);
						}
						break;
					}
					default :
						if (SDL_BlitSurface(sonIconeInconnu, NULL, sonEcran, &laPosition) == -1)
						{
							std::cerr << "SDL_BlitSurface(): " << SDL_GetError() << std::endl;
							exit(EXIT_FAILURE);
						}

				}

				leNom = (*leItX)->getSonNom();

				if (leNom.size() > name_max_size)
				{
					leNom = leNom.substr(0, name_max_size);
				}

				ecrire(leNom, laPosition.x + icon_width / 2.0 - ((leNom.size()) / 2.0) * (font_size / 2.0), laPosition.y + icon_height, leX == sonX && leY == sonY);
			}

			leX++;
		}

		leX = 0;
		leY++;
	}

	if (sonMenu != 0)
	{

		if (SDL_BlitSurface(sonFondMenu, NULL, sonEcran, NULL) == -1)
		{
			std::cerr << "SDL_BlitSurface(): " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		ecrire("->", 200, (2 + sonMenu) * font_size, false);
		ecrire("Open", 212, 3 * font_size, false);
		ecrire("Copy", 212, 4 * font_size, false);
		ecrire("Paste", 212, 5 * font_size, false);
		ecrire("Paste Into Folder", 212, 6 * font_size, false);
		ecrire("Delete", 212, 7 * font_size, false);
		ecrire("Shut Down", 212, 11 * font_size, false);
		ecrire("Restart", 212, 12 * font_size, false);
	}

	if (sonExplorateur.getSesErreurs() != "")
	{
		std::istringstream lesErreurs(sonExplorateur.getSesErreurs());
		std::string lErreur;
		int i = 0;

		if (SDL_BlitSurface(sonFondMessage, NULL, sonEcran, NULL) == -1)
		{
			std::cerr << "SDL_BlitSurface(): " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		while (lesErreurs.good())
		{
			getline(lesErreurs, lErreur);

			if (lErreur != "")
			{
				ecrire(lErreur, 160.0 - lErreur.size() / 2.0 * (font_size / 2.0), (3.0 + i) * font_size, false);
				i++;
			}
		}

		ecrire("A : OK", 200, 240 - font_size, false);
	}

	if (SDL_Flip(sonEcran) == -1)
	{
		std::cerr << "SDL_Flip(): " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	return;
}

SDL_Surface * Interface::getSonIcone(std::string uneExtension) const
{
	SDL_Surface * lIcone = NULL;
	std::map<std::string, SDL_Surface *>::const_iterator leIt = sesIcones.find(uneExtension);

	if (leIt != sesIcones.end())
	{
		lIcone = leIt->second;
	}

	return lIcone;
}

SDL_Surface * Interface::chargerImage(std::string unFichier) const
{
	SDL_Surface * lImage = IMG_Load(unFichier.c_str());

	if (lImage == NULL)
	{
		std::cerr << "IMG_Load(): " << IMG_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	return lImage;
}

void Interface::ecrire(std::string unTexte, int unX, int unY, bool estSelectionne)
{
	SDL_Rect laPosition;
	SDL_Surface * leTexte;

	if (estSelectionne == true)
	{
		laPosition.x = unX;
		laPosition.y = unY;
		leTexte = TTF_RenderText_Shaded(saPolice, unTexte.c_str(), sonBlanc, sonFocus);

		if (leTexte == NULL)
		{
			std::cerr << "TTF_RenderText_Shaded(): " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		if (SDL_BlitSurface(leTexte, NULL, sonEcran, &laPosition) == -1)
		{
			std::cerr << "SDL_BlitSurface(): " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

	}

	else
	{
		laPosition.x = unX + 1;
		laPosition.y = unY + 1;
		leTexte = TTF_RenderText_Blended(saPolice, unTexte.c_str(), sonNoir);

		if (leTexte == NULL)
		{
			std::cerr << "TTF_RenderText_Blended(): " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		if (SDL_BlitSurface(leTexte, NULL, sonEcran, &laPosition) == -1)
		{
			std::cerr << "SDL_BlitSurface(): " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		laPosition.x = unX;
		laPosition.y = unY;
		leTexte = TTF_RenderText_Blended(saPolice, unTexte.c_str(), sonBlanc);

		if (leTexte == NULL)
		{
			std::cerr << "TTF_RenderText_Blended(): " << TTF_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

		if (SDL_BlitSurface(leTexte, NULL, sonEcran, &laPosition) == -1)
		{
			std::cerr << "SDL_BlitSurface(): " << SDL_GetError() << std::endl;
			exit(EXIT_FAILURE);
		}

	}

	SDL_FreeSurface(leTexte);
	return;
}

void Application::eteindre() const
{
	TTF_Quit();
	SDL_Quit();
	execlp("poweroff", "poweroff", NULL);
	return;
}

void Application::ouvrirFichier(Element * unFichier)
{
	char exe[512];
	int exe_len = readlink("/proc/self/exe", exe, sizeof(exe) - 1);
	exe[exe_len] = '\0';
	std::string cwd(getcwd(NULL, 0));
	std::string leNom = unFichier->getSonNom();
	std::string lExtension = unFichier->getSonExtension();

	if (lExtension == "dge" || lExtension == "sh")
	{
		std::string laCommande = "\"./" + unFichier->getSonNom() + "\"; cd \"" + cwd + "\"; exec \"" + exe + "\";";

		if (chdir(sonRepertoireCourant.c_str()) == -1)
		{
			ajouterErreur("Permission denied.");
		}

		else
		{
			TTF_Quit();
			SDL_Quit();
			execlp("/bin/sh", "/bin/sh", "-c", laCommande.c_str(), NULL);
		}

	}

	else
	{
		std::string lApplication = getSonApplication(lExtension);

		if (lApplication.empty())
		{
			ajouterErreur("Could not open the file.");
		}

		else
		{
			int leSlash = lApplication.find_last_of('/');
			std::string leRepertoire = lApplication.substr(0, leSlash);
			lApplication = lApplication.substr(leSlash + 1);
			std::string laCommande = "\"./" + lApplication + "\" \"" + sonRepertoireCourant + unFichier->getSonNom() + "\"; cd \"" + cwd + "\"; exec \"" + exe + "\";";

			if (chdir(leRepertoire.c_str()) == -1)
			{
				ajouterErreur("Permission denied.");
			}

			else
			{
				TTF_Quit();
				SDL_Quit();
				execlp("/bin/sh", "/bin/sh", "-c", laCommande.c_str(), NULL);
			}

		}

	}

	return;
}

