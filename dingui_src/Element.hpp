#include <dirent.h>
#include <string>

enum type { dossier, fichier, inconnu };

class Element
{
	std::string sonNom, sonExtension;
	type sonType;
public :
	Element(dirent *);
	std::string getSonNom() const;
	std::string getSonExtension() const;
	type getSonType() const;
};

