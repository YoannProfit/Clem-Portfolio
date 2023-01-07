//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : IOPorts.h
// Date de création : Octobre 2015
// Description : Une classe pour gérer et envoyer de
// l'information sur les ports d'entrée / sortie
//----------------------------------------------------

#ifndef IO_PORTS_H
#define IO_PORTS_H

#include "Types.h"

class IOPorts
{
public:
	IOPorts();
	virtual ~IOPorts();

private:
	void OutputByte(const uint32 iAddress, const int8 iValue);
	void OutputWord(const uint32 iAddress, const int16 iValue);
	void OutputLong(const uint32 iAddress, const int32 iValue);

	const uint8 InputByte(const uint32 iAddress); 
	const uint16 InputWord(const uint32 iAddress);
	const uint32 InputLong(const uint32 iAddress);
	
	void EnableInterrupts();
	void DisableInterrupts();
};

#endif