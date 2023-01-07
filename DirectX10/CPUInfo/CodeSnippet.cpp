#include "CPUInfo.h"
 
CPUInfo::CPUInfo() :

    m_iProcessorCoreCount(0),
    m_iPhysicalProcessorCount(0),
    m_iLogicalProcessorCount(0),
    m_iProcessorL1CacheCount(0),
    m_iProcessorL2CacheCount(0),
    m_iProcessorL3CacheCount(0),
    m_iProcessorL1CacheSize(0),
    m_iProcessorL2CacheSize(0),
    m_iProcessorL3CacheSize(0)
  
{
}
 
CPUInfo::~CPUInfo()
{
}
 
bool CPUInfo::Initialize()
{
    if (!ParseProcessorData())
    {
        return false;
    }
 
    ParseSystemProcessorCount();
 
    return true;
}
 
bool CPUInfo::ParseProcessorData()
{
    m_iLogicalProcessorCount = 0;
    m_iPhysicalProcessorCount = 0;
 
    LPFN_GLPI Glpi;
 
    Glpi = (LPFN_GLPI) GetProcAddress(GetModuleHandle(TEXT("kernel32")),
        "GetLogicalProcessorInformation");
     
    if (Glpi == nullptr)
    {
        /* La fonction qui permet d'obtenir les informations
           du CPU n'est pas supportée */
        return false;
    }
 
    bool bDone = false;
 
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pBuffer = nullptr;
    DWORD iReturnLength = 0;
 
    while (!bDone)
    {
        bool bSucess = Glpi(pBuffer, &iReturnLength);
 
        if (!bSucess)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (pBuffer)
                {
                    free(pBuffer);
                }
 
                pBuffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION) malloc(iReturnLength);
 
                if (pBuffer == nullptr)
                {
                    // Echec de l'allocation
                    return false;
                }
            }
            else
            {
                // Erreur inconnu
                return false;
            }
        }
        else
        {
            bDone = true;
        }
    }
 
    DWORD iByteOffset = 0;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = pBuffer;
    PCACHE_DESCRIPTOR pCache = nullptr;
 
    while (iByteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= iReturnLength)
    {
        switch (ptr->Relationship)
        {
            case RelationProcessorCore:
                {
                    // Nombre de processeurs physiques
                    m_iPhysicalProcessorCount++;
 
                    // Nombre de processeurs logiques (ceux utilisable par le système)
                    m_iLogicalProcessorCount += CountBits(ptr->ProcessorMask);
 
                    break;
                }
            case RelationCache:
                {
                    pCache = &ptr->Cache;
 
                    if (pCache->Level == 1)
                    {
                        m_iProcessorL1CacheCount++;
 
                        m_iProcessorL1CacheSize = pCache->Size;
                    }
                    else if (pCache->Level == 2)
                    {
                        m_iProcessorL2CacheCount++;
 
                        m_iProcessorL2CacheSize = pCache->Size;
                    }
                    else if (pCache->Level == 3)
                    {
                        m_iProcessorL3CacheCount++;
 
                        m_iProcessorL3CacheSize = pCache->Size;
                    }
 
                    break;
                }
        }
 
        iByteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }
 
    free (pBuffer);
 
    return true;
}
 
uint32 CPUInfo::CountBits(uint64 bitMask)
{
    DWORD c = 0;
 
    while (bitMask != 0)
    {
        c += (bitMask & 1L);
        bitMask >>= 1;
    }
 
    return c;
}
 
void CPUInfo::ParseSystemProcessorCount()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
 
    m_iProcessorCoreCount = (uint16) si.dwNumberOfProcessors;
}
 
uint16 CPUInfo::GetSystemProcessorCount()
{
    return m_iProcessorCoreCount;
}
 
uint16 CPUInfo::GetPhysicalProcessorCount()
{
    return m_iPhysicalProcessorCount;
}
 
uint16 CPUInfo::GetLogicalProcessorCount()
{
    return m_iLogicalProcessorCount;
}
 
uint16 CPUInfo::GetL1CacheCount()
{
    return m_iProcessorL1CacheCount;
}
 
uint16 CPUInfo::GetL2CacheCount()
{
    return m_iProcessorL2CacheCount;
}
 
uint16 CPUInfo::GetL3CacheCount()
{
    return m_iProcessorL3CacheCount;
}
