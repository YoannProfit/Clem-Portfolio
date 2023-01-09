//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : GlobalDescriptorTable.h
// Date de création : Octobre 2015
// Description : La GDT sert de descripteur de
// segments de code (un segment est défini sur 64 bits)
//
// Les segments peuvent être de type 'system',
// 'code' ou 'data'.
//----------------------------------------------------

#ifndef GLOBAL_DESCRIPTOR_MANAGER_H
#define GLOBAL_DESCRIPTOR_MANAGER_H

#include "Singleton.h"
#include "Types.h"
#include "Defines.h"
#include "DescriptorTable.h"

/*  
 Le descripteur de segment a pour taille 
 totale 64 bits.

 Ce descripteur précise une base (l'endroit en mémoire 
 où commence le segment), une limite (taille du segment 
 exprimée en octets) et un type.
 

 Voici la structure générale d'un segment (dans l'ordre) : 

	(on part du bit de celui de poids le plus faible 
	c'est-à-dire le bit commençant tout à droite)
	
	- Les 16 premiers bits de la taille du segment ('limit')
	- Les 16 premiers bits de la base du segment ('base')
	- Les 8 suivants bits de la base du segment
	- Les 8 bits d'accès 
	- Les 8 bits du segment 'granularity'
	- Les 8 derniers bits de la base du segment
	 
*/
struct SegmentDescriptor
{
	uint16 			iLimitLow;
	uint16 			iBaseLow;
	uint8 			iBaseMiddle;
	uint8 			iAccess;
	uint8 			iGranularity;
	uint8 			iBaseHigh;
} GEM_PACK;


struct GDTRegister
{
	uint16 			iLimitSize;
	uint32 			iBaseAdress; // L'adresse de la première entrée de la GDT
} GEM_PACK;

// ----------------------------------- //

/* Une classe pour créer un descripteur 
   de segments mémoire à la volée */
class DescriptorBuilder : public StructureBuilder
{
public:
	StructureBuilder();
	virtual ~StructureBuilder();

	void SetLimit();
	void SetBase();
	void SetAccess();
	void SetGranularity();

	SegmentDescriptor* GetDescriptor();
	
	/* Réinitialise la configuration du
	   descripteur */
	void Reset();
	
	// Construit la structure finale
	virtual void Build();

private:
	SegmentDescriptor* m_pDescriptor;

	uint32 m_iLimit;
	uint32 m_iBase;
	uint8 m_iAccess;
	uint8 m_iGranularity;
};

// ----------------------------------- //

/* Une classe pour établir la GDT et créer les différents
   descripteurs (code, données) automatiquement */
class SegmentDescriptorManager : public Singleton<SegmentDescriptorManager>
{
public:
	SegmentDescriptorManager();
	virtual ~SegmentDescriptorManager();

	bool Initialize();

private:
	void CreateGDT();

	void CreateNullDescriptor();
	void CreateCodeDescriptor();
	void CreateDataDescriptor();
	void CreateStackDescriptor();
		
	void SetupGDTRegister();

private:
	DescriptorTable<SegmentDescriptor*> m_GDT;

	DescriptorBuilder m_descriptorBuilder;
};

#endif