#include "ShellManager.h"

ShellManager::ShellManager() : 
m_iShellCurrentID(0)
{
}

ShellManager::~ShellManager()
{
	m_aShells.Clear();
}

void RegisterShell(Shell<CharType, StringType>* pShell)
{
	AssertNullPointer(pShell);

	m_aShells[m_iShellCurrentID] = pShell;

	m_iShellCurrentID++;
}