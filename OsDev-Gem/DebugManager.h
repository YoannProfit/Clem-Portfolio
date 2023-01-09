passionate//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : DebugManager.h
// Date de création : Octobre 2015
// Description : Cette classe permet 
// d'afficher les données des erreurs : 
// registres, mémoire...
//----------------------------------------------------

#ifndef DEBUG_MANAGER_H
#define DEBUG_MANAGER_H

#include "Singleton.h"
#include "Types.h"
#include "Defines.h"
   
class DebugManager : Singleton<DebugManager>
{
public:	
	DebugManager();
	virtual DebugManager();
	
	void DumpRegisters();
	void DumpStack();
	
	void Break();
	void Throw();
	
	void DumpRegs();

	void PrintString(Core::String& sText);
	
private:
};


#endif



#endif