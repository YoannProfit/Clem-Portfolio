#include "System.h"
#include "IOPorts.h"


System::System() :
m_pCurrentTTY(nullptr)
{
}

System::~System()
{
}

bool System::Initialize()
{
	CreateTTYs();
}

void System::HaltCPU()
{
	asm("hlt");
}

void System::Reboot()
{
	uint8 iGood = 0x02;
	
	/* On attend que le buffer de sortie du système de ports
	   soit plein */
	while ((iGood & 0x02) != 0)
	{
		iGood = GEM_IO_PORTS->InputByte(0x64);
	}

	// On reboot la machine
	GEM_IO_PORTS->OutputByte(0x64, GEM_REBOOT_VALUE);
}


bool System::AddProcess(Kernel::Process* pProcess)
{
	m_aProcesses[pName] = pProcess;
}

bool System::DestroyProcess(Kernel::Process* pProcess)
{
	m_aProcesses.Remove(pName);
}

void System::SetCurrentTTY(TTY* pCurrentTTY)
{
	if (m_pCurrentTTY != nullptr)
	{
		/* On désactive la reception des entrées
		   utilisateur pour l'ancien TTY */
		m_pCurrentTTY->DisableInput();
	}

	m_pCurrentTTY = pCurrentTTY;

	m_pCurrentTTY->EnableInput();

	// On affiche le contenu du présent TTY
	m_pCurrentTTY->RefreshLines();
}

void System::SoloKeyPressed(const KeyEvent& arg)
{
	if (GEM_INPUT_MANAGER->IsModifierDown(InputManager::KeyModifier::CTRL)
		&& GEM_INPUT_MANAGER->IsModifierDown(InputManager::KeyModifier::ALT))
	{
		uint8 iTTYNumber = 0;

		switch (arg.key)
		{
		case GEM_KEY_F1:
			iTTYNumber = 1;
			break;
		case GEM_KEY_F2:
			iTTYNumber = 2;
			break;
		case GEM_KEY_F3:
			iTTYNumber = 3;
			break;
		case GEM_KEY_F4:
			iTTYNumber = 4;
			break;
		case GEM_KEY_F5:
			iTTYNumber = 5;
			break;
		case GEM_KEY_F6:
			iTTYNumber = 6;
			break;
		case GEM_KEY_F7:
			iTTYNumber = 7;
			break;
		default:
			iTTYNumber = 0;
			break;
		};

		if (iTTYNumber != 0)
		{
			TTY* pTTY = m_aTTYs[iTTYNumber];

			// On change de TTY
			SetCurrentTTY(pTTY);
		}
	}

}

void System::SoloKeyReleased(const KeyEvent& arg)
{
}

void System::CreateTTYs()
{
	AssertMsg(m_aTTYs.Size() == 0, "TTYs déjà crées !");

	for (uint8 i = 0; i < GEM_TTYS_COUNT; i++)
	{
		TTY* pTTY = new TTY();
		pTTY->PrintAnnounce();
		pTTY->HidePrompt();

		m_aTTYs.PushBack(pTTY);
	}

	TTY* pFirstTTY = m_aTTYs[0];
	SetCurrentTTY(pFirstTTY);
}