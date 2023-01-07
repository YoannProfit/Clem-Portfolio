#include "Keyboard.h"
#include "IOPorts.h"

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}
/***

When the IRQ happens, we call this handler which reads from port 0x60



void keyboard_read_key()
{
	lastkey = 0;
	if(inportb(0x64) & 1)
		lastkey = inportb(0x60);
}


***/

void Keyboard::WaitUntilReady()
{
	while (true)
	{
		if (GEM_IO_PORTS->InputByte(0x64 & 2) == 0)
		{
			break; 
		}
	}
}