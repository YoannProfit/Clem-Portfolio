#ifndef COMPUTER_INFO_H
#define COMPUTER_INFO_H

namespace fw
{
	class ComputerInfo : public Singleton<ComputerInfo>
	{
	public:
		struct SPCWindowsInformation
		{
			std::string WindowsName;
			std::string WindowsVersion;
		};

		ComputerInfo();
		virtual ~ComputerInfo();

		void ParseOSName();
		void ParseCPU();
		void ParseMemory();
		void ParseHDSpace();

		DLLEXPORT const std::string& GetOsName();
		DLLEXPORT const std::string& GetCpuName();
		DLLEXPORT const std::string& GetCpuSpeed();
		DLLEXPORT const std::string& GetTotalMemory();
		DLLEXPORT const std::string& GetAvailableMemory();
		DLLEXPORT const std::string& GetHDSpace();

		void GetWindowsInformations(SPCWindowsInformation& informations);

	private:
		bool CompareWindowsVersionMinor(const DWORD dwMinorVersion);
		bool CompareWindowsVersionMajor(const DWORD dwMajorVersion);

		bool IsWindows10();
		bool IsWindows11();

	private:
		std::string m_sOsName;
		std::string m_sCpuName;
		std::string m_sCpuSpeed;
		std::string m_sTotalMemory;
		std::string m_sAvailableMemory;
		std::string m_sHDDAvailableMemory;

		std::mutex m_mutex;
	};
}

#endif