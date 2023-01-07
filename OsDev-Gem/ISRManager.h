//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier :  ISRManager.h
// Date de création : Octobre 2015
// Description : Cette classe permet 
// de définir les 'Interrupts Service Routines'
//----------------------------------------------------

#ifndef INTERRUPT_SERVICE_ROUTINES_MANAGER_H
#define INTERRUPT_SERVICE_ROUTINES_MANAGER_H

#include "Singleton.h"
#include "Types.h"
#include "Defines.h"

class ISRRoutine
{
public:
	ISRRoutine();
	virtual ~ISRRoutine();

	(void*) GetFunction();

private:
	uint8 m_iRoutineNumber;
};

class ISRManager : Singleton<ISRManager>
{
public:	
	/*
		0 - Division by zero exception
		1 - Debug exception
		2 - Non maskable interrupt
		3 - Breakpoint exception
		4 - 'Into detected overflow'
		5 - Out of bounds exception
		6 - Invalid opcode exception
		7 - No coprocessor exception
		8 - Double fault (pushes an error code)
		9 - Coprocessor segment overrun
		10 - Bad TSS (pushes an error code)
		11 - Segment not present (pushes an error code)
		12 - Stack fault (pushes an error code)
		13 - General protection fault (pushes an error code)
		14 - Page fault (pushes an error code)
		15 - Unknown interrupt exception
		16 - Coprocessor fault
		17 - Alignment check exception
		18 - Machine check exception	
		19-31 - Reserved
	*/
	enum InterruptServiceRoutines
	{
		DivByZero,
		Debug,
		NonMaskable,
		Breakpoint,
		...
	};

	ISRManager();
	virtual ISRManager();
	
	ISRRoutine* GetISR(uint8 iISRNumber);
	ISRRoutine* GetDefaultISR();

	bool HasISR(uint8 id);

private:
	ISRRoutine* m_aRoutines[GEM_IDT_ENTRIES_COUNT];
};


// ----------------------------------- //

REGISTER_ISR(DivByZero);
REGISTER_ISR(Debug);
REGISTER_ISR();
REGISTER_ISR();


DECLARE_ISR(DivByZero)
{
}

DECLARE_ISR()
{
}

DECLARE_ISR()
{
}

DECLARE_ISR()
{
}

DECLARE_ISR()
{
}

#endif
