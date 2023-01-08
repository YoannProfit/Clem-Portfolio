//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : ShellManager.h
// Date de création : Octobre 2015
// Description : Gestionnaire des différents shells
// (TTY, GraphicTerminal, Shell, etc...)
//----------------------------------------------------

#ifndef SHELL_MANAGER_H
#define SHELL_MANAGER_H

#include "Types.h"
#include "Array.h"
#include "Singleton.h"
#include "Shell.h"

class ShellManager : public Singleton<ShellManager>
{
public:
	ShellManager()
	virtual ~ShellManager();

	template<typename CharType, typename StringType>
	void RegisterShell(Shell<CharType, StringType>* pShell);

private:
	Core::Map<uint32, Shell<CharType, StringType>*> m_aShells;

	uint32 m_iShellCurrentID;
};

#endif