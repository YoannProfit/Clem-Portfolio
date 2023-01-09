//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : Logger.h
// Date de création : Octobre 2015
// Description : Classe pour enregistrer tous les 
// messages systèmes de l'OS.
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