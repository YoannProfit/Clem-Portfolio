/* How to retrieve computer's information - Code Snippet */

#ifndef COMPUTER_INFO_H
#define COMPUTER_INFO_H
 
class ComputerInfo
{
public:
    ComputerInfo();
    ~ComputerInfo();
 
    void parseOSName();
    void parseCPU();
    void parseMemory();
    void parseHDSpace();
     
    std::string getOsName();
    std::string getCpuName();
    std::string getCpuSpeed();
    std::string getTotalMemory();
    std::string getAvailableMemory();
 
private:
    std::string m_sOsName;
    std::string m_sCpuName;
    std::string m_sCpuSpeed;
    std::string m_sTotalMemory;
    std::string m_sAvailableMemory;
};
 
#endif
