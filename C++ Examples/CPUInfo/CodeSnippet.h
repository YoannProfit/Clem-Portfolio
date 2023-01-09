#ifndef CPU_INFO_H
#define CPU_INFO_H
 
#include <Windows.h>
#include <iostream>
 
#include "Defines.h"
#include "Types.h"
 
typedef bool (WINAPI *LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
 
class CPUInfo    
{
public:
    CPUInfo();
    virtual ~CPUInfo();
     
    bool Initialize();
 
    uint16 GetLogicalProcessorCount();
    uint16 GetPhysicalProcessorCount();
 
    uint16 GetSystemProcessorCount();
 
    uint16 GetL1CacheCount();
    uint16 GetL2CacheCount();
    uint16 GetL3CacheCount();
 
private:
    void ParseSystemProcessorCount();
    bool ParseProcessorData();
	
    uint32 CountBits(uint64 bitMask);
 
private:
    uint16 m_iPhysicalProcessorCount;
    uint16 m_iLogicalProcessorCount;
    uint16 m_iProcessorCoreCount;
 
    uint16 m_iProcessorL1CacheCount;
    uint16 m_iProcessorL2CacheCount;
    uint16 m_iProcessorL3CacheCount;
 
    size_t m_iProcessorL1CacheSize;
    size_t m_iProcessorL2CacheSize;
    size_t m_iProcessorL3CacheSize;
};
 
#endif
