//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : System.h
// Date de création : Octobre 2015
// Description : Classe pour gérer le système de l'OS.
//----------------------------------------------------

#ifndef SYSTEM_H
#define SYSTEM_H

#include "Singleton.h"
#include "Map.h"

class System : public Singleton<System>, public KeyListener
{
public:
	System();
	virtual ~System();

	bool Initialize();		

	void Reboot();	
	void Shutdown();
	void HaltCPU();

	void DetectCPUInfo();

	Kernel::Pid CreateProcess(const char* sFileName, uint32 argc, char** argv);
	void KillProcess(const kernel::Pid& pid);

	TTY* GetCurrentTTY();
	void SetCurrentTTY(TTY* pCurrentTTY);

	void MainLoop();

	virtual void SoloKeyPressed(const KeyEvent<char>& arg);
	virtual void SoloKeyReleased(const KeyEvent<char>& arg);

	void CreateTTYs();

private:
	// Kernel:: et User::
	bool AddProcess(Kernel::Process* pProcess);
	bool DestroyProcess(Kernel::Process* pProcess);

private:
	Core::Map<Core::String, Kernel::Process*> m_aProcesses;
	Core::Array<TTY*> m_aTTYs;

	TTY* m_pCurrentTTY;
};

#endif