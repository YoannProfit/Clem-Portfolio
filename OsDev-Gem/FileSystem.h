//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : FileSystem.h
// Date de création : Octobre 2015
// Description : Une classe qui gère et organise
// les fichiers entre eux
//----------------------------------------------------

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "File.h"

class Filesystem
{
public:
	Filesystem();
	virtual ~Filesystem();
		
	bool Initialize();

	File* GetPath(const char* sPath);
	File* GetPathParent(const char* sPath, const char *sFileName);
		
	uint32 Link(const char* sFileName, const char* sNewFileName);
		
	uint32 AddFile(const char* sDirectory, File* pFilePath);
		
	File* PivotRoot(File* targetdir);
		
	File* GetRoot();
		
private:
	File* m_pRoot;
	File* m_pDev;
	File* m_pVar;
};
#endif
