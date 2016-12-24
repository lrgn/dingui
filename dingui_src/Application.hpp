#include "Element.hpp"
#include <map>
#include <vector>

class Application
{
	std::map<std::string, std::string> saConfiguration, sesApplications;
	std::string sonCopier, sonRepertoireCourant, sesErreurs;
	std::vector<std::vector<Element *> > sonContenu;
public :
	Application();
	~Application();
	void ajouterErreur(std::string);
	void changerRepertoire(std::string);
	void coller(std::string) const;
	void copier(std::string);
	void eteindre() const;
	std::string getSaValeur(std::string) const;
	std::string getSesErreurs() const;
	std::string getSonApplication(std::string) const;
	std::vector<std::vector<Element *> > getSonContenu() const;
	void lireRepertoire();
	void ouvrirFichier(Element *);
	void redemarrer() const;
	void resetSesErreurs();
	void supprimer(std::string) const;
};

