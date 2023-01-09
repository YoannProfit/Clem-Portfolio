#include "GlobalDescriptorTable.h"

/*************** DescriptorBuilder ***************/

DescriptorBuilder::DescriptorBuilder() :
m_pDescriptor(nullptr),
m_bBuilt(false),
m_iLimit(0),
m_iBase(0),
m_iAccess(0),
m_iGranularity(0)
{
}

DescriptorBuilder::~DescriptorBuilder()
{
}

void DescriptorBuilder::SetLimit(uint32 iLimit)
{
	m_iLimit = iLimit;
}

void DescriptorBuilder::SetBase(uint32 iBase)
{
	m_iBase = iBase;
}

void DescriptorBuilder::SetAcces(uint8 iAccess)
{
	m_iAccess = iAccess;
}

void DescriptorBuilder::SetGranularity(uint8 iGranularity)
{
	m_iGranularity = iGranularity;
}

SegmentDescriptor* DescriptorBuilder::GetDescriptor()
{
	AssertMsg(m_bBuilt, "Le descripteur doit être construit avant d'y accéder !");
	
	return m_pDescriptor;
}

void DescriptorBuilder::Reset()
{
	SAFE_DELETE(m_pDescriptor);
	
	m_pDescriptor = new SegmentDescriptor();

	m_bBuilt = false;
}

void DescriptorBuilder::Build()
{
	Reset();
	
	AddUnsignedInt16("Limit");
	AddUnsignedInt16("BaseLow");
	AddUnsignedInt8("BaseMiddle");
	AddUnsignedInt8("Access");
	AddUnsignedInt8("Granularity");	
	AddUnsignedInt8("BaseHigh");

	StructureBuilder::Build();

	SetValue("Limit", GetLow16(m_iLimit));
	SetValue("BaseLow", GetLow16(m_iBase));
	SetValue("BaseMiddle", GetMiddle8(m_iBase));
	SetValue("Access",  m_iAccess);
	SetValue("Granularity", GetLow16(m_iGranularity));
	SetValue("BaseHigh", GetHigh8(m_iBase));
	
	m_pDescriptor = reinterpret_cast<SegmentDescriptor*>( GetStructure() );
	
	m_bBuilt = true;
}


/*************** SegmentDescriptorManager ***************/

SegmentDescriptorManager::SegmentDescriptorManager()
{
}

SegmentDescriptorManager::~SegmentDescriptorManager()
{
}

bool SegmentDescriptorManager::Initialize()
{
	CreateGDT();

	CreateNullDescriptor();
	CreateCodeDescriptor();
	CreateDataDescriptor();
	CreateStackDescriptor();

	return true;
}


void SegmentDescriptorManager::CreateGDT()
{
	AssertMsg(m_pGDT == nullptr, "GDT déjà créée!");
}

void SegmentDescriptorManager::CreateNullDescriptor()
{
	m_descriptorBuilder.Reset();

	m_descriptorBuilder.SetLimit(0);
	m_descriptorBuilder.SetBase(0);
	m_descriptorBuilder.SetAccess(0);
	m_descriptorBuilder.SetGranularity(0);

	m_descriptorBuilder.Build();

	SegmentDescriptor* pNullDescriptor = m_descriptorBuilder.GetDescriptor();

	m_GDT.AddSegmentDescriptor(pNullDescriptor);
}

void SegmentDescriptorManager::CreateCodeDescriptor()
{
	m_descriptorBuilder.Reset();

	m_descriptorBuilder.SetLimit(GEM_CODE_LIMIT);
	m_descriptorBuilder.SetBase(GEM_CODE_BASE);
	m_descriptorBuilder.SetAccess(GEM_CODE_ACCESS);
	m_descriptorBuilder.SetGranularity(GEM_CODE_GRANULARITY);

	m_descriptorBuilder.Build();

	SegmentDescriptor* pCodeDescriptor = m_descriptorBuilder.GetDescriptor();

	m_GDT.AddSegmentDescriptor(pCodeDescriptor);
}

void SegmentDescriptorManager::CreateDataDescriptor()
{
	m_descriptorBuilder.Reset();

	m_descriptorBuilder.SetLimit(GEM_DATA_LIMIT);
	m_descriptorBuilder.SetBase(GEM_DATA_BASE);
	m_descriptorBuilder.SetAccess(GEM_DATA_ACCESS);
	m_descriptorBuilder.SetGranularity(GEM_DATA_GRANULARITY);

	m_descriptorBuilder.Build();

	SegmentDescriptor* pDataDescriptor = m_descriptorBuilder.GetDescriptor();

	m_GDT.AddSegmentDescriptor(pDataDescriptor);
}

void SegmentDescriptorManager::CreateStackDescriptor()
{
	AssertNullPointer(m_pGDT);

	m_descriptorBuilder.Reset();

	m_descriptorBuilder.SetLimit(GEM_STACK_LIMIT);
	m_descriptorBuilder.SetBase(GEM_STACK_BASE);
	m_descriptorBuilder.SetAccess(GEM_STACK_ACCESS);
	m_descriptorBuilder.SetGranularity(GEM_STACK_GRANULARITY);

	m_descriptorBuilder.Build();

	SegmentDescriptor* pStackDescriptor = m_descriptorBuilder.GetDescriptor();

	m_GDT.AddSegmentDescriptor(pStackDescriptor);
}

void SegmentDescriptorManager::SetupGDTRegister()
{
	GDTRegister gdtRegister;

	gdtRegister.iLimitSize = sizeof(SegmentDescriptor) * (m_GDT.GetDescriptorCount())
	gdtRegister.iBaseAdress = m_GDT.GetFirstEntry();

    // Chargement du registre
    asm("lgdtl (gdtRegister)");

	// ----------------------------------- //

	// Initialisation des segments
	asm("movw $0x10, %ax   \n \
		 movw %ax, %ds     \n \
		 movw %ax, %es     \n \
		 movw %ax, %fs     \n \
		 movw %ax, %gs     \n \
		 ljmp $0x08, $next \n \
		 next:             \n");


	
}