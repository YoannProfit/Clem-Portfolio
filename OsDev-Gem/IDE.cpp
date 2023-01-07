#include "IDE.h"

IDE::IDE()
{
}

IDE::~IDE()
{
}


void IDE::WaitUntilReady(uint16 iBase)
{
	while (GEM_IO_PORTS->InputByte(iBase + 0x206) & 0x80);
}