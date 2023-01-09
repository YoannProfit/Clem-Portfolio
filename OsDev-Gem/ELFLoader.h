//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : ELFLoader.h
// Date de création : Octobre 2015
// Description : Une classe pour gérer les fichiers
// éxécutables au format ELF
//----------------------------------------------------

#ifndef ELF_LOADER_H
#define ELF_LOADER_H

#include "Defines.h"
#include "Types.h"
#include "Array.h"
#include "Singleton.h"

/* En-tête d'un fichier éxecutable de format ELF */
struct ELFHeader
{
	uchar id[GEM_ELF_IDENT_SIZE];			/* Identification ELF */
	
	uint16 iType;							/* 2 (exec file) */
	uint16 iMachine;						/* 3 (intel architecture) */
	uint32 iVersion;						/* 1 */
	uint32 iEntry;							/* starting point */
	 
	uint32 iProgramHeaderOff;				/* program header table offset */
	uint32 iSectionHeaderOff;				/* section header table offset */
	uint32 iFlags;							/* various flags */
	uint16 iHeaderSize;						/* ELF header (this) size */

	uint16 iPhentsize;						/* program header table entry size */
	uint16 iPhnum;							/* number of entries */

	uint16 iShentsize;						/* section header table entry size */
	uint16 iShnum;							/* number of entri	es */

	uint16 iShstrndx;						/* index of the section name string table */
};

class ELFLoader : public Singleton<ELFLoader>
{
public:
	ELFLoader();
	virtual ~ELFLoader();
	
	bool IsELF(const char* pFileBuffer);
	
	bool CheckCompatibility();

	uint32 LoadELF(const char* sFileName, ProcessInfo* pProcessInfo);

	/*
	int Execv(char* file,int argc,char** argv);
	void Execv_module(u32 entry,int argc,char** argv);
	*/

private:
};

#endif