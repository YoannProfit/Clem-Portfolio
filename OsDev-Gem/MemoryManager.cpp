#include "MemoryManager.h"

MemoryManager::MemoryManager() :
m_iTotalAllocatedSize(0)
{
}

MemoryManager::~MemoryManager()
{
}

void MemoryManager::KernelAlloc(size_t iAllocatedSize)
{
}

void MemoryManager::KernelCAlloc(size_t iAllocatedSize)
{
}

bool MemoryManager::HasReachedTotalLimit()
{
}

void MemoryManager::KernelFree(void* pAddress)
{
	if (pAddress == nullptr)
	{
		return;
	}
}
