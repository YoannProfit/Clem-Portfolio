//----------------------------------------------------
// Auteur : Cl�ment Profit
// Nom du fichier : Logger.h
// Date de cr�ation : Octobre 2015
// Description : Classe pour enregistrer tous les 
// messages syst�mes de l'OS.
//----------------------------------------------------

#ifndef LOGGER_H
#define LOGGER_H

#include "Types.h"
#include "Array.h"
#include "Singleton.h"

class Logger : public Singleton<Logger>
{
public:
	Logger();
	virtual ~Logger();

private:
	FileSystem* m_pFileSystem;
};


#endif