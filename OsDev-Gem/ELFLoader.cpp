#include "ELFLoader.h"

template<> ELFLoader* Singleton<ELFLoader>::ms_instance = nullptr;

ELFLoader::ELFLoader()
{
}

ELFLoader::~ELFLoader()
{
}

bool ELFLoader::IsELF(const char* pFileBuffer)
{
	const ELFHeader* pHeader = (ELFHeader*) pFileBuffer;
	
	return (pHeader->id[0] == GEM_ELF_HEADER_START_MAGIC_NUMBER
		&& pHeader->id[1] == GEM_ELF_HEADER_START_MAGIC_NUMBER_ELF_1
		&& pHeader->id[2] == GEM_ELF_HEADER_START_MAGIC_NUMBER_ELF_2
		&& pHeader->id[3] == GEM_ELF_HEADER_START_MAGIC_NUMBER_ELF_3);
}