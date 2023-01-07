//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : Page.h
// Date de création : Octobre 2015
// Description : Classe qui représente la gestion d'un 
// utilisateur du système
//----------------------------------------------------

#ifndef USER_H
#define USER_H

#include "Types.h"

namespace Core
{
	class User
	{
		public:

			void SetPassword(char* sName);
			char* GetPassword();
		
		protected:
			u32 utype;

			char password[512];
	};
};

#endif
