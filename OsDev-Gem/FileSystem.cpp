#include "FileSystem.h"

Filesystem::Filesystem()
{
}

Filesystem::~Filesystem()
{
	delete m_pRoot;
}

bool Filesystem::Initialize()
{
	m_pRoot = new File("/", FileType::TYPE_DIRECTORY);
	
	// Dossier contenant les peripherique
	m_pDev = m_pRoot->CreateChild("dev", FileType::TYPE_DIRECTORY);

	// Dossier contenant les processus tournant
	m_pRoot->CreateChild("proc", FileType::TYPE_DIRECTORY);		

	// Dossier contenant les points de montages des disques
	m_pRoot->CreateChild("mnt", FileType::TYPE_DIRECTORY);		

	// Dossier contenant toutes les infos du systemes
	File* pSysDirectory = m_pRoot->CreateChild("sys", FileType::TYPE_DIRECTORY);

	// Dossier contenant toutes les variables d'environnement
	m_pVar = pSysDirectory->CreateChild("env", FileType::TYPE_DIRECTORY);	

	// Dossier contenant tous les utilisateurs
	pSysDirectory->CreateChild("usr", FileType::TYPE_DIRECTORY);	

	// Dossier contenant tous les modules disponiles
	pSysDirectory->CreateChild("mods", FileType::TYPE_DIRECTORY);

	// Dossier contenant tous les sockets actuels
	pSysDirectory->CreateChild("sockets", FileType::TYPE_DIRECTORY);
}

File* Filesystem::GetPath(const char* sPath)
{
	File* pFilePath = m_pRoot;
}

File* Filesystem::GetRoot()
{
	return m_pRoot;
}

uint32 Filesystem::AddFile(const char* sDirectory, File* pFilePath)
{
	File* pDirectory = Path(sDirectory);

	if (pDirectory == NULL)
	{
		return GEM_ERROR;
	}
	else
	{
		return pDirectory->AddChild(pFilePath);
	}
}