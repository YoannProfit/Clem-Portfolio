//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : Page.h
// Date de création : Octobre 2015
// Description : Classe qui représente un utilisateur
// du système
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