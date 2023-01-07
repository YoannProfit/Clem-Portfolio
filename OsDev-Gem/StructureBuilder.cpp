#include <stdlib.h>

#include "StructureBuilder.h"

StructureBuilder::StructureBuilder(size_t iSize = 0) :
m_pStructure(nullptr),
m_bBuilt(false)
{
	m_iSize = iSize;
	
	m_pStructure = malloc(iSize);
}

StructureBuilder::~StructureBuilder()
{
}

void StructureBuilder::AddUnsignedInt8(const Core::String& sName)
{
	m_aFields[sName] = sizeof(uint8);
}

void StructureBuilder::AddUnsignedInt16(const Core::String& sName)
{
	m_aFields[sName] = sizeof(uint16);
}

void StructureBuilder::AddUnsignedInt32(const Core::String& sName)
{
	m_aFields[sName] = sizeof(uint32);
}

void StructureBuilder::AddUnsignedInt64(const Core::String& sName)
{
	m_aFields[sName] = sizeof(uint64);
}	

void StructureBuilder::AddUnsignedChar(const Core::String& sName)
{
	m_aFields[sName] = sizeof(uchar);
}

void StructureBuilder::AddInt8(const Core::String& sName)
{
	m_aFields[sName] = sizeof(int8);
}

void StructureBuilder::AddInt16(const Core::String& sName)
{
	m_aFields[sName] = sizeof(int16);
}

void StructureBuilder::AddInt32(const Core::String& sName)
{
	m_aFields[sName] = sizeof(int32);
}

void StructureBuilder::AddInt64(const Core::String& sName)
{
	m_aFields[sName] = sizeof(int64);
}	

void StructureBuilder::ComputeSize()
{
	Core::Map<Core::String, uint8>::Iterator it;
	
	size_t iComputedSize = 0;
	
	for (it = m_aFields.begin(); it != m_aFields.end(); it++)
	{
		size_t iCurrentSize = it->GetValue();
		
		iComputedSize += iCurrentSize;	
	}
	
	m_iSize = iComputedSize;
}

size_t StructureBuilder::GetSize()
{
	return m_iSize;
}

void StructureBuilder::AddPadding(size_t iPaddingSize)
{
	m_aFields["Padding"] = iPaddingSize;
}

void StructureBuilder::Build()
{
	m_pStructure = malloc( GetSize() );

	m_bBuilt = true;
}

char* StructureBuilder::GetStructure()
{
	AssertMsg(m_bBuilt, "Le descripteur doit être configuré avant d'y accéder !");
	
	return m_pStructure;
}

const uint16 StructureBuilder::GetLow16(const uint32 iInteger)
{
	return (iInteger & 0xFFFF);
}

const uint8 StructureBuilder::GetMiddle8(const uint32 iInteger)
{
	return ((iInteger >> 16) & 0xFF);
}

const uint8 StructureBuilder::StructureBuilder::GetHigh8(const uint32 iInteger)
{
	return (iInteger >> 24) & 0xFF;
}

const uint16 StructureBuilder::StructureBuilder::GetHigh16(const uint32 iInteger)
{
	return (iInteger >> 16) & 0xFFFF);
}

#endif