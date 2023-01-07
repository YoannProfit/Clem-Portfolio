#include "InterruptDescriptor.h"

/*************** InterruptTable ***************/

InterruptBuilder::InterruptDescriptorBuilder() :
m_pDescriptor(nullptr),
m_bBuilt(false),
m_iLimit(0),
m_iBase(0),
m_iAccess(0),
m_iOther(0)
{
}

InterruptBuilder::~InterruptDescriptorBuilder()
{
}

void InterruptBuilder::SetSelector(uint16 iSelect)
{
	m_iSelect = iSelect;
}

void InterruptBuilder::SetOffset(uint32 iOffset)
{
	m_iOffset = iOffset;
}

void InterruptBuilder::SetType(uint16 iType)
{
	m_iType = iType;
}

SegmentDescriptor* InterruptBuilder::GetDescriptor()
{
	AssertMsg(m_bBuilt, "Le descripteur doit être configuré avant d'y accéder !");
	
	return m_pDescriptor;
}

void InterruptBuilder::Reset()
{
	SAFE_DELETE(m_pDescriptor);
	
	m_pDescriptor = new InterruptDescriptor();

	m_bBuilt = false;
}

void InterruptBuilder::Build()
{
	Reset();
	
	AddUnsignedInt16("OffsetLow");
	AddUnsignedInt16("Select");
	AddUnsignedInt16("Type");
	AddUnsignedInt16("OffsetHigh");
	
	StructureBuilder::Build();

	SetValue("OffsetLow", GetLow16( m_iOffset ));
	SetValue("Select", m_iSelect);
	SetValue("Type", m_iType);
	SetValue("OffsetHigh", GetHigh16( m_iOffset ));
	
	m_pDescriptor = reinterpret_cast<InterrruptDescriptor*>( GetStructure() );
	
	m_bBuilt = true;
}

/*************** InterruptDescriptorManager ***************/

InterruptDescriptorManager::InterruptDescriptorManager()
{
}

InterruptDescriptorManager::~InterruptDescriptorManager()
{
}

bool InterruptDescriptorManager::Initialize()
{
	CreateIDT();

	SetupISRs();

	SetupIDTRegister();

	return true;
}

void InterruptDescriptorManager::CreateIDT()
{
	for (uint32 i = 0; i < GEM_IDT_ENTRIES_NUMBER; i++)
	{
		m_descriptorBuilder.Reset();

		ISRRoutine* pISRFunction = nullptr;

		bool bISRExists = GEM_ISR_MANAGER->HasISR(i);

		if (bISRExist)
		{
			pISRFunction = GEM_ISR_MANAGER->GetISR(i);
		}
		else
		{
			pISRFunction = GEM_ISR_MANAGER->GetDefaultISR();
		}

		AssertNullPointer(pISRFunction);

		m_descriptorBuilder.SetOffset(0);
		m_descriptorBuilder.SetSelector( pISRFunction );
		m_descriptorBuilder.SetType(0);

		m_descriptorBuilder.Build();

		InterruptDescriptor* pIDTDescriptor = m_descriptorBuilder.GetDescriptor();
		
		m_IDT.AddSegmentDescriptor(pIDTDescriptor);
	}
}

void InterruptDescriptorManager::SetupISRs()
{
}

void InterruptDescriptorManager::SetupIDTRegister()
{
	IDTRegister idtRegister;

	idtRegister.iLimitSize = sizeof(InterruptDescriptor) * (m_IDT.GetDescriptorCount())
	idtRegister.iBaseAdress = m_IDT.GetFirstEntry();

	asm("lidtl (idtRegister)");
}