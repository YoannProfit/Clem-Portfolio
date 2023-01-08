//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : MemoryManager.h
// Date de création : Octobre 2015
// Description : Une classe pour gérer les allocations
// de mémoire de l'OS.
//----------------------------------------------------

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H


#include "Singleton.h"
#include "Types.h"

class MemoryManager : public Singleton<MemoryManager>
{
public:
	MemoryManager();
	virtual ~MemoryManager();
	
	void* KernelAlloc(size_t iAllocatedSize);
	void* KernelCAlloc(size_t iAllocatedSize);
	
	void KernelFree(void* pAdress);

	bool HasReachTotalMemoryLimit();

	Page* GetFreePage();

private:
	uint64 m_iTotalAllocatedSize;
};

void* operator new(size_t iSize)
{
    return GEM_MEMORY_MANAGER->Alloc(iSize);
}
 
void* operator new[](size_t iSize)
{
    return GEM_MEMORY_MANAGER->Alloc(iSize);
}
 
void operator delete(void *p)
{
	return GEM_MEMORY_MANAGER->Free(iSize);
}
 
void operator delete[](void *p)
{
	return GEM_MEMORY_MANAGER->Free(iSize);
}


#endif