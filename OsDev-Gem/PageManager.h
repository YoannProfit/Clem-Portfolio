//----------------------------------------------------
// Auteur : Cl�ment Profit
// Nom du fichier : PageManager.h
// Date de cr�ation : Octobre 2015
// Description : Classe qui g�re les pages de
// la m�moire vive
//----------------------------------------------------

#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include "Types.h"
#include "List.h"
#include "Singleton.h"

class Page
{
public:
	Page();
	virtual ~Page();

	size_t GetReferenceCount();
	uint32* GetAdress();

	void IncrementReferenceCount();
	void DecrementReferenceCount();

private:
	uint32* m_pPageAdress;
	uint32 m_iReferenceCount;
};

class PageManager : public Singleton<PageManager>
{
public:
	PageManager();
	virtual ~PageManager();

	size_t GetFreePagesCount();
	size_t GetUsedPagesCount();

private:
	Core::List<Page*> m_aFreePages;
	Core::List<Page*> m_aUsedPages;
};


#endif