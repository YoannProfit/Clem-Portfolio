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
			User();
			virtual ~User();
			
			void SetPassword(char* sName);
			char* GetPassword();
		
		protected:
			uint32 m_iType;

			char m_sPassword[512];
	};
};

#endif
