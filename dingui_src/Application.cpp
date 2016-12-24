#include "Application.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

Application::Application()
:sonCopier("")
,sonRepertoireCourant("/boot/")
,sesErreurs("")
{
	std::ifstream laConfiguration("config.txt");
	std::string laVariable, laValeur;

	while (laConfiguration.good() == true)
	{
		getline(laConfiguration, laVariable, '=');
		getline(laConfiguration, laValeur, '\n');

		if (laVariable == "" || laValeur == "" || saConfiguration.insert(std::make_pair(laVariable, laValeur)).second == false)
		{
			std::cerr << "Application(): " << "The config.txt file is corrupted" << std::endl;
			exit(EXIT_FAILURE);
		}

	}

	laConfiguration.close();
	std::ifstream lesApplications("apps.txt");
	std::string lExtension, lApplication;

	while (lesApplications.good() == true)
	{
		getline(lesApplications, lExtension, '\t');
		getline(lesApplications, lApplication, '\n');

		if (lExtension == "" || lApplication == "" || sesApplications.insert(std::make_pair(lExtension, lApplication)).second == false)
		{
			ajouterErreur("The apps.txt file is corrupted.");
		}

	}

	lesApplications.close();
	std::vector<std::vector<Element *> >::iterator leItY;
	std::vector<Element *>::iterator leItX;
	sonContenu.resize(1000);

	for (leItY = sonContenu.begin(); leItY != sonContenu.end(); leItY++)
	{
		leItY->resize(atoi(getSaValeur("nb-icons-x").c_str()));

		for (leItX = leItY->begin(); leItX != leItY->end(); leItX++)
		{
			*leItX = NULL;
		}

	}

}

Application::~Application()
{
	std::vector<std::vector<Element *> >::iterator leItY;
	std::vector<Element *>::iterator leItX;

	for (leItY = sonContenu.begin(); leItY != sonContenu.end(); leItY++)
	{

		for (leItX = leItY->begin(); leItX != leItY->end(); leItX++)
		{
			delete *leItX;
		}

	}

}

void Application::ajouterErreur(std::string uneErreur)
{
	sesErreurs += uneErreur + '\n';
	return;
}

void Application::changerRepertoire(std::string unRepertoire)
{

	if (unRepertoire == "..")
	{

		if (sonRepertoireCourant != "/")
		{
			int i = sonRepertoireCourant.find_last_of('/', sonRepertoireCourant.size() - 2);
			sonRepertoireCourant.erase(i + 1);
		}

	}

	else
	{
		sonRepertoireCourant += unRepertoire + '/';
	}

	return;
}

void Application::coller(std::string unRepertoire) const
{

	if (sonCopier != "")
	{
		std::string laCommande = "cp -Rf \"" + sonCopier + "\" \"" + sonRepertoireCourant + unRepertoire + "\";";
		system(laCommande.c_str());
	}

	return;
}

void Application::copier(std::string unFichier)
{
	sonCopier = sonRepertoireCourant + unFichier;
	return;
}

std::string Application::getSaValeur(std::string uneVariable) const
{
	std::map<std::string, std::string>::const_iterator leIt = saConfiguration.find(uneVariable);

	if (leIt == saConfiguration.end())
	{
		std::cerr << "getSaValeur(): " << "The config.txt file is corrupted" << std::endl;
		exit(EXIT_FAILURE);
	}

	return leIt->second;
}

std::string Application::getSesErreurs() const
{
	return sesErreurs;
}

std::string Application::getSonApplication(std::string uneExtension) const
{
	std::string uneApplication = "";
	std::map<std::string, std::string>::const_iterator leIt = sesApplications.find(uneExtension);

	if (leIt != sesApplications.end())
	{
		uneApplication = leIt->second;
	}

	return uneApplication;
}

std::vector<std::vector<Element *> > Application::getSonContenu() const
{
	return sonContenu;
}

void Application::lireRepertoire()
{
	std::vector<std::vector<Element *> >::iterator leItY;
	std::vector<Element *>::iterator leItX;

	for (leItY = sonContenu.begin(); leItY != sonContenu.end(); leItY++)
	{

		for (leItX = leItY->begin(); leItX != leItY->end(); leItX++)
		{
			delete *leItX;
			*leItX = NULL;
		}

	}

	DIR * leRepertoire = opendir(sonRepertoireCourant.c_str());

	if (leRepertoire == NULL)
	{
		ajouterErreur("Permission denied.");
		changerRepertoire("..");
		lireRepertoire();
	}

	else
	{
		dirent * lEntree = NULL;
		int leX = 0;
		int leXMax = atoi(getSaValeur("nb-icons-x").c_str());
		int leY = 0;
		bool estVide = true;

		while ((lEntree = readdir(leRepertoire)) != NULL)
		{

			if (leX == leXMax)
			{
				leX = 0;
				leY++;
			}

			if (sizeof(lEntree->d_name) != 0 && lEntree->d_name[0] != '.')
			{
				estVide = false;
				sonContenu[leY][leX] = new Element(lEntree);
				leX++;
			}

		}

		closedir(leRepertoire);

		if (estVide == true)
		{
			ajouterErreur("There are no files.");
			changerRepertoire("..");
			lireRepertoire();
		}

	}

	return;
}

void Application::redemarrer() const
{
	execlp("reboot", "reboot", NULL);
	return;
}

void Application::resetSesErreurs()
{
	sesErreurs.clear();
	return;
}

void Application::supprimer(std::string unFichier) const
{
	std::string laCommande = "rm -Rf \"" + sonRepertoireCourant + unFichier + "\";";
	system(laCommande.c_str());
	return;
}

