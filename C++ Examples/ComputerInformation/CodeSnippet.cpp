/* How to retrieve computer's information - C++ Code Snippet */

using namespace fw;

template<> ComputerInfo* Singleton<ComputerInfo>::ms_instance = nullptr;

ComputerInfo::ComputerInfo()
{
	ParseOSName();
	ParseCPU();
	ParseMemory();
	ParseHDSpace();
}

ComputerInfo::~ComputerInfo()
{
}

const std::string& ComputerInfo::GetOsName()
{
	return m_sOsName;
}

const std::string& ComputerInfo::GetCpuName()
{
	return m_sCpuName;
}

const std::string& ComputerInfo::GetCpuSpeed()
{
	return m_sCpuSpeed;
}

const std::string& ComputerInfo::GetTotalMemory()
{
	return m_sTotalMemory;
}

const std::string& ComputerInfo::GetAvailableMemory()
{
	return m_sAvailableMemory;
}

const std::string& ComputerInfo::GetHDSpace()
{
	return m_sHDDAvailableMemory;
}

bool ComputerInfo::IsWindows10()
{
	HKEY hKey;

	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		0,
		KEY_READ,
		&hKey);

	DWORD BufSize = _MAX_PATH;
	std::string sProductName;
	sProductName.resize(256);

	RegQueryValueExA(hKey, "ProductName", NULL, NULL, (LPBYTE)&sProductName[0], &BufSize);

	if (STRING_BUILDER->StartWith(sProductName, "Windows 10"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ComputerInfo::CompareWindowsVersionMinor(const DWORD dwMinorVersion)
{
	OSVERSIONINFOEX ver;
	DWORDLONG dwlConditionMask = 0;

	ZeroMemory(&ver, sizeof(OSVERSIONINFOEX));
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	ver.dwMinorVersion = dwMinorVersion;

	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);

	return VerifyVersionInfo(&ver, VER_MINORVERSION, dwlConditionMask);
}

bool ComputerInfo::CompareWindowsVersionMajor(const DWORD dwMajorVersion)
{
	OSVERSIONINFOEX ver;
	DWORDLONG dwlConditionMask = 0;

	ZeroMemory(&ver, sizeof(OSVERSIONINFOEX));
	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	ver.dwMajorVersion = dwMajorVersion;

	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);

	return VerifyVersionInfo(&ver, VER_MAJORVERSION, dwlConditionMask);
}

void ComputerInfo::ParseOSName()
{
	std::string sOSName = "OS Inconnu";
	
	if (CompareWindowsVersionMajor(10))
	{
		if ((CompareWindowsVersionMinor(0)))
		{
			sOSName = "Windows 10.0";
		}
	}
	else if (CompareWindowsVersionMajor(6))
	{
		if ((CompareWindowsVersionMinor(3)))
		{
			sOSName = "Windows 8.1";
		}
		else if ((CompareWindowsVersionMinor(2)))
		{
			if (IsWindows10())
			{
				sOSName = "Windows 10.0";
			}
			else
			{
				sOSName = "Windows 8";
			}
		}
		else if ((CompareWindowsVersionMinor(1)))
		{
			sOSName = "Windows 7";
		}
		else if ((CompareWindowsVersionMinor(0)))
		{
			sOSName = "Windows Vista";
		}
	}
	else if (CompareWindowsVersionMajor(5))
	{
		if (CompareWindowsVersionMinor(1))
		{
			sOSName = "Windows XP";
		}
	}

	m_sOsName = sOSName;
}

void ComputerInfo::ParseCPU()
{
    int CPUInfo[4] = {-1};
    unsigned nExIds;
	char CPUBrandString[0x40] = { 0 };

    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];

    for (uint32 i = 0x80000000; i <= nExIds; ++i)
    {
    	__cpuid(CPUInfo, i);

    	if  (i == 0x80000002)
    		memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
    	else if  (i == 0x80000003)
    		memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
    	else if  (i == 0x80000004)
    		memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }

	m_sCpuName = STRING_BUILDER->RemoveSpaceAfterString(CPUBrandString);

	/************************************/
	//char Buffer[_MAX_PATH];
	DWORD BufSize = _MAX_PATH;
	DWORD dwMHz = _MAX_PATH;
	HKEY hKey;

	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                        "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                        0,
                        KEY_READ,
                        &hKey);
#if 0
    if (lError != ERROR_SUCCESS)
    {
           FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
                         NULL,
                         lError,
                         0,
                         Buffer,
                         _MAX_PATH,
                         0);
    }
#endif

    RegQueryValueExA(hKey, "~MHz", NULL, NULL, (LPBYTE) &dwMHz, &BufSize);

	char string[512] = { 0 };
	
	sprintf(string, "%i", dwMHz);

	m_sCpuSpeed = std::string(string) + " MHz";
}

void ComputerInfo::ParseMemory()
{
	MEMORYSTATUSEX status;
	
	status.dwLength = sizeof(status);

    GlobalMemoryStatusEx(&status);

    __int64 totalPhysicalMem = 0;
	__int64 availableMem = 0;

    totalPhysicalMem = status.ullTotalPhys / 1024;
    totalPhysicalMem = (totalPhysicalMem / 1024) + 1;

    availableMem = status.ullAvailPhys / 1024;
    availableMem = (availableMem / 1024) + 1;

    m_sTotalMemory = STRING_BUILDER->IntegerToString((int) totalPhysicalMem) + " MB";
    m_sAvailableMemory = STRING_BUILDER->IntegerToString((int) availableMem) + " MB";
}

void ComputerInfo::ParseHDSpace()
{
	int const drive = _getdrive();

	struct _diskfree_t diskfree;
	_getdiskfree(drive, &diskfree);

	unsigned __int64 const total = diskfree.sectors_per_cluster * diskfree.bytes_per_sector * diskfree.avail_clusters;

	m_sHDDAvailableMemory = STRING_BUILDER->IntegerToString((int)total / 1024 / 1024) + " GB";
}

void ComputerInfo::GetWindowsInformations(SPCWindowsInformation& informations)
{
	//return; // FIX bug thread

	// Always call Begin() method before doing any operation with WINDOWS_API_QUERIES
	// because of the multithreading concurency issues

	//WINDOWS_API_QUERIES->Begin();

	WINDOWS_API_QUERIES->InitializeWSQLQueries();

	//m_mutex.try_lock();

		WINDOWS_API_QUERIES->WSQLQuery(L"select * from Win32_OperatingSystem");

	//m_mutex.unlock();

		WindowsAPIQueries::QueryResult result;
		WINDOWS_API_QUERIES->WSQLFetchArray(result);

	// Always call End() after any operation
	//WINDOWS_API_QUERIES->End();

	if (result.count(L"Version") > 0)
	{
		informations.WindowsVersion = result[L"Version"].GetFirstParameterAsString();
	}

	if (result.count(L"Caption") > 0)
	{
		informations.WindowsName = result[L"Caption"].GetFirstParameterAsString();
	}
}
