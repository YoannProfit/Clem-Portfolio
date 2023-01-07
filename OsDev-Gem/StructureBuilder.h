//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : StructureBuilder.h
// Date de création : Octobre 2015
// Description : Permet de construire une structure
// dynamiquement.
//----------------------------------------------------

#ifndef STRUCTUE_BUILDER_H
#define STRUCTUE_BUILDER_H

#include "Types.h"
#include "Defines.h"

class StructureBuilder
{
public:
	StructureBuilder(size_t iSize);
	virtual ~StructureBuilder();

	/** Adders */
	void AddUnsignedInt8(const Core::String& sName);
	void AddUnsignedInt16(const Core::String& sName);
	void AddUnsignedInt32(const Core::String& sName);
	void AddUnsignedInt64(const Core::String& sName);	
	void AddUnsignedChar(const Core::String& sName);
	
	void AddInt8(const Core::String& sName);
	void AddInt16(const Core::String& sName);
	void AddInt32(const Core::String& sName);
	void AddInt64(const Core::String& sName);	
	void AddChar(const Core::String& sName);
	
	/** Setters **/
	void SetValue(const Core::String& sName, const uint8);
	void SetValue(const Core::String& sName, const uint16);
	void SetValue(const Core::String& sName, const uint32);
	void SetValue(const Core::String& sName, const uint64);
	void SetValue(const Core::String& sName, const uchar);
	
	void SetValue(const Core::String& sName, const int8);
	void SetValue(const Core::String& sName, const int16);
	void SetValue(const Core::String& sName, const int32);
	void SetValue(const Core::String& sName, const int64);	
	void SetValue(const Core::String& sName, const char);		
	
	size_t GetSize();
	
	void AddPadding(size_t iPaddingSize);
	
	virtual void Build();

	void ComputeSize();

	const uint16 GetLow16(const uint32 iInteger);
	const uint8 GetMiddle8(const uint32 iInteger);
	const uint8 GetHigh8(const uint32 iInteger);
	const uint16 GetHigh16(const uint32 iInteger);	

private:
	size_t m_iSize;
	
	char* m_pStructure;
	
	/* Tableau qui associe un nom d'attribut au 
	   nombre d'octets alloués */
	Core::Map<Core::String, size_t> m_aFields;
	
	bool m_bBuilt;
};

#endif