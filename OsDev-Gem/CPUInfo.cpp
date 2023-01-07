#include "CPUInfo.h"

CPUInfo::CPUInfo() :
m_iMaxValueEAX(0),
m_vendor(NotSet)
{
	m_aVendorStrings = 
	{
		"GenuineIntel",
		"AuthenticAMD",
		"Unknown",
	};
	
	m_aVendorCodes =
	{
		// Intel
		{0x756e6547, 0x6c65746e, 0x49656e69},	// "uneG", "letn", "Ieni"
		// AMD
		{0x68747541, 0x444d4163, 0x69746e65},	// "htuA", "DMAc", "itne"
		// Unknown
		{0x0, 0x0, 0x0},
	};
}

CPUInfo::~CPUInfo()
{
}

bool CPUInfo::DoCPUID(uint32 in, uint32 out[4])
{
	if(in > m_iMaxValueEAX && (in < GEM_MAX_EXT_VALUE_EAX || in > GEM_MAX_EXT_VALUE_EAX))
	{
		return false;
	}

	asm volatile("cpuid" : "=a"(out[0]), "=b"(out[1]), "=c"(out[2]), "=d"(out[3]) : "a"(in));

	return true;
}


void CPUInfo::FindVendor()
{
	uint32 aRegisters[4] = {0, 0, 0, 0};

	cpuid(0x0, aRegisters);

	// update maxValue for eax
	m_iMaxValueEAX = aRegisters[0];
	
	// Intel check
	if (aRegisters[1] == m_aVendorCodes[0][0] &&
		aRegisters[2] == m_aVendorCodes[0][1] &&
		aRegisters[3] == m_aVendorCodes[0][2])
	{
		vendor = Vendor::Intel;
	}
	// AMD check
	else if (aRegisters[1] == m_aVendorCodes[1][0] &&
			aRegisters[2] == m_aVendorCodes[1][1] &&
			aRegisters[3] == m_aVendorCodes[1][2])
	{
		m_vendor = Vendor::AMD;
	}
	else
	{
		m_vendor = Vendor::Unknown;
	}
	
	GetMaxExtValueEax();
}

void CPUInfo::GetMaxExtValueEax()
{
	uint32 aRegisters[4] = {0, 0, 0, 0};
	
	DoCPUID(0x80000000, aRegisters);
	
	m_iMaxExtValueEAX = aRegisters[0];
}
