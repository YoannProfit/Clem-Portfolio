/* How to retrieve computer's information - Code Snippet */

#include <direct.h>
#include "ComputerInfo.h"
 
ComputerInfo::ComputerInfo()
{
    parseOSName();
    parseCPU();
    parseMemory();
}
 
ComputerInfo::~ComputerInfo()
{
}
 
std::string ComputerInfo::getOsName()
{
    return m_sOsName;
}
 
std::string ComputerInfo::getCpuName()
{
    return m_sCpuName;
}
 
std::string ComputerInfo::getCpuSpeed()
{
    return m_sCpuSpeed;
}
 
std::string ComputerInfo::getTotalMemory()
{
    return m_sTotalMemory;
}
 
std::string ComputerInfo::getAvailableMemory()
{
    return m_sAvailableMemory;
}
 
void ComputerInfo::parseOSName()
{
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
 
    std::string sOSName;
 
    switch(osvi.dwMajorVersion)
    {
        case 6:
            {
                if (osvi.dwMinorVersion == 3)
                    sOSName = "Windows 8.1";
                else if (osvi.dwMinorVersion == 2)
                    sOSName = "Windows 8";
                else if (osvi.dwMinorVersion == 1)
                    sOSName = "Windows 7";
                else if (osvi.dwMinorVersion == 0)
                    sOSName = "Windows Vista";
            }
            break;
        case 5:
            {
                if (osvi.dwMinorVersion == 1)
                    sOSName = "Windows XP";
            }
            break;
 
        default: sOSName = "OS Inconnu";
    }
 
    m_sOsName = sOSName;
}
 
void ComputerInfo::parseCPU()
{
    int CPUInfo[4] = {-1};
    unsigned   nExIds, i =  0;
    char CPUBrandString[0x40];
 
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    for (i=0x80000000; i<=nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
 
        if  (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }
 
    m_sCpuName = CPUBrandString;
 
    /************************************/
    char Buffer[_MAX_PATH];
    DWORD BufSize = _MAX_PATH;
    DWORD dwMHz = _MAX_PATH;
    HKEY hKey;
 
    long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
            "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
             0,
             KEY_READ,
             &hKey);
     
    if(lError != ERROR_SUCCESS)
    {
           FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                         NULL,
                         lError,
                         0,
                         Buffer,
                         _MAX_PATH,
                         0);
    }
 
    RegQueryValueEx(hKey, "~MHz", NULL, NULL, (LPBYTE) &dwMHz,
                    &BufSize);
 
    char string[512] = { 0 };
     
    sprintf(string, "%i", dwMHz);
 
    m_sCpuSpeed = std::string(string) + " MHz";
}
 
void ComputerInfo::parseMemory()
{
    MEMORYSTATUSEX status;
     
    status.dwLength = sizeof(status);
 
    GlobalMemoryStatusEx(&status);
 
    int totalPhysicalMem = 0;
    int availableMem = 0;
 
    totalPhysicalMem = (int) status.ullTotalPhys / 1024;
    totalPhysicalMem = (totalPhysicalMem / 1024) + 1;
 
    availableMem = (int) status.ullAvailPhys / 1024;
    availableMem = (availableMem / 1024) + 1;
 
    m_sTotalMemory = MiscManager::intToString(totalPhysicalMem) +
                   " MB";
    m_sAvailableMemory = MiscManager::intToString(availableMem) +
                       " MB";
}
