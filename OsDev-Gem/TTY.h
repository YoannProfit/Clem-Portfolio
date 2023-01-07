//----------------------------------------------------
// Auteur : Cl�ment Profit
// Nom du fichier : TTYManager.h
// Date de cr�ation : Octobre 2015
// Description : Cette classe permet de g�rer les 
// diff�rentes consoles que le syst�me diposent 
// en appuyant (F1..)
//----------------------------------------------------

#ifndef TTY_H
#define TTY_H

#include "Singleton.h"
#include "Types.h"
#include "Defines.h"
#include "Map.h"

class TTY : public Shell
{
public:
	TTY();
	virtual ~TTY();

	void PrintAnnonce();

private:
	bool m_bIsLoggedAsRoot;
};

#endif