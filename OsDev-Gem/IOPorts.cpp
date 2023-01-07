#include "IOPorts.h"

void IOPorts::OutputByte(const uint32 iAddress, const int8 iValue)
{
	asmv("outb %%al, %%dx" :: "d" (iAddress), "a" (iValue));
}

void IOPorts::OutputWord(const uint32 iAddress, const int16 iValue)
{
	asmv("outw %%ax, %%dx" :: "d" (iAddress), "a" (iValue));
}

void IOPorts::OutputLong(const uint32 iAddress, const int32 iValue)
{
	asmv("outl %%eax, %%dx" : : "d" (iAddress), "a" (iValue));
}

// ------------------------------------ //

const uint8 IOPorts::InputByte(const uint32 iAddress)
{
	uint8 iValue;   
	
	asmv("inb %%dx, %%al" : "=a" (iValue) : "d" (iAddress)); 
	
	return iValue;
}
 
const uint16 IOPorts::InputWord(const uint32 iAddress)
{
	uint16 iValue;  
	
	asmv("inw %%dx, %%ax" : "=a" (iValue) : "d" (iAddress));
	
	return iValue;
}

const uint32 IOPorts::InputLong(const uint64 iAddress)
{
	uint32 iValue;  
	
	asmv("inl %%dx, %%eax" : "=a" (iValue) : "d" (iAddress));	
	
	return iValue;
}

// ------------------------------------ //

void IOPorts::EnableInterrupts()
{
	asm("sti");
}

void IOPorts::DisableInterrupts()
{
	asm("cli");
}
