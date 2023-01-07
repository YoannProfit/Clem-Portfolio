//----------------------------------------------------
// Auteur : Cl�ment Profit
// Nom du fichier : InterruptlDescriptorTable.h
// Date de cr�ation : Octobre 2015
// Description : La IDT se charge d�finir les routines
// d'interruption qui doivent �tre appel�es
//----------------------------------------------------

#ifndef INTERRUPT_DESCRIPTOR_MANAGER_H
#define INTERRUPT_DESCRIPTOR_MANAGER_H

#include "Types.h"
#include "Defines.h"
#include "DescriptorTable.h"

/*  
 Le descripteur d'interruption a pour taille 
 totale 64 bits.

 Ce descripteur pr�cise le s�lecteur ('select', une entr�e de la GDT),
 un d�calage ('offset') et son type.
 
 Structure g�n�rale d'un segment d'interruption (dans l'ordre) : 
 
	- Les 16 premiers bits (en partant de celui de poids faible) de la valeur de d�calage (offset)
	- L'adresse de la routine InterruptRoutineService
	- Les 16 bits du type
	- Les 16 dernier bits de la valeur de d�calage
	 
*/
struct InterruptDescriptor 
{
    uint16 iOffsetLow;    
    uint16 iSelect;
    uint16 iType;
    uint16 iOffsetHigh;    
} GEM_PACK;

struct IDTRegister
{
	uint16 iLimitSize;
	uint32 iBaseAdress;
} GEM_PACK;

// ------------------------------------ //

/*
 Une classe pour cr�er un descripteur 
 d'interruption � la vol�e

 /!\ Doit �tre instancier sur la pile 
 lors de l'utilisation.
*/
class InterruptionBuilder : public StructureBuilder
{
public:
	InterruptionBuilder();
	virtual ~InterruptionBuilder();
	
	void SetSelector(uint16 iSelect);
	void SetOffset(uint32 iOffset);
	void SetType(uint16 iType);

	InterruptDescriptor* GetDescriptor();
	
	/* R�initialise la configuration du
	   descripteur */
	void Reset();
	
	// Construit la structure finale
	virtual void Build();
	
private:
	InterruptDescriptor* m_pDescriptor;

	uint16 m_iSelect;
	uint32 m_iOffset;
	uint16 m_iType;
};

// ------------------------------------ //

/* Une classe pour �tablir la IDT et cr�er les diff�rents
   descripteurs d'interruption automatiquement */
class InterruptDescriptorManager : public Singleton<InterruptDescriptorManager>
{
public:
	InterruptDescriptorManager();
	virtual ~InterruptDescriptorManager();

	bool Initialize();

	void CreateIDT();

	void SetupISRs();

	void SetupIDTRegister();

private:
	DescriptorTable<InterruptDescriptor*> m_IDT;

	InterruptionBuilder m_descriptorBuilder;
};

#endif