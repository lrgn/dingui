#include "Element.hpp"

Element::Element(dirent * uneEntree)
:sonNom(uneEntree->d_name)
,sonExtension("")
,sonType(inconnu)
{

	switch (uneEntree->d_type)
	{
		case DT_DIR :
			sonType = dossier;
			break;
		case DT_REG :
			sonType = fichier;
			break;
		default :
			sonType = inconnu;
	}

	if (sonType == fichier)
	{
		int lePoint = sonNom.find_last_of('.');
		sonExtension = sonNom.substr(lePoint + 1);
	}

}

std::string Element::getSonNom() const
{
	return sonNom;
}

std::string Element::getSonExtension() const
{
	return sonExtension;
}

type Element::getSonType() const
{
	return sonType;
}

