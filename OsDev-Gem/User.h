//----------------------------------------------------
// Auteur : Cl�ment Profit
// Nom du fichier : Page.h
// Date de cr�ation : Octobre 2015
// Description : Classe qui repr�sente un utilisateur
// du syst�me
//----------------------------------------------------

#ifndef USER_H
#define USER_H

#include "Types.h"
namespace Core
{
	class User
	{
		void	setPassword(char *n);
		char*	getPassword();

				
		protected:
		u32		utype;

		char	password[512];
	}
};
#endif