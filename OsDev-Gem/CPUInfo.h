//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : CPUInfo.h
// Date de création : Octobre 2015
// Description : Une classe pour obtenir des informations
// sur l'architecture du processeur machine
//----------------------------------------------------

#ifndef CPU_INFORMATION_H
#define CPU_INFORMATION_H

#include "Singleton.h"
#include "Types.h"

class CPUInfo : public Singleton<CPUInfo>
{
public:
	enum class Vendor : uint8
	{
		Intel = 0,
		AMD = 1,
		Unknown = 2,
		NotSet = 3,
	};
	
	CPUInfo();
	virtual ~CPUInfo();
	
private:	
	bool DoCPUID(uint32 in, uint32 out[4]);

	void FindVendor();
	
	void GetMaxExtValueEax();
	
private:
	const char* m_aVendorStrings[3];
	const uint32 m_aaVendorCodes[3][3];
	
	uint16 m_iMaxValueEAX;
	
	Vendor m_vendor;
};

#endif