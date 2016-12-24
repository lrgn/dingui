#include "Element.hpp"
#include <map>
#include <vector>

class Application
{
	std::map<std::string, std::string> saConfiguration, sesApplications;
	std::string sesErreurs, sonCopier, sonRepertoireCourant;
	std::vector<std::vector<Element *> > sonContenu;
public :
	Application();
	~Application();
	void ajouterErreur(std::string);
	void changerRepertoire(std::string);
	void coller(std::string);
	void copier(std::string);
	void eteindre() const;
	std::string getSaValeur(std::string) const;
	std::string getSesErreurs() const;
	std::string getSonApplication(std::string) const;
	std::vector<std::vector<Element *> > getSonContenu() const;
	std::string getSonRepertoireCourant() const;
	void lireRepertoire();
	void ouvrirFichier(Element *, bool);
	void redemarrer() const;
	void resetSesErreurs();
	void supprimer(std::string);
};

