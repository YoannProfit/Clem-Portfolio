//----------------------------------------------------
// Auteur : Cl�ment Profit
// Nom du fichier : Keyboard.h
// Date de cr�ation : Octobre 2015
// Description : Cette classe clavier permet 
// d'�couter les entr�es des touches tapp�es par 
// l'utilisateur
//----------------------------------------------------

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Types.h"
#include "Defines.h"

/*
Commandes : 

 	0xEE (Echo) ou 
	0xFA (ACK) ou 
	0xFE (Resend) 

*/
class Keyboard //: public Device
{
public:
	enum KeyLight
	{
		ScrollLock = BIT(0);
		NumLock = BIT(1);
		CapsLock = BIT(2);
	};

	Keyboard();
	virtual ~Keyboard();

	void WaitUntilReady();

	void EnableKeyLight();
	void DisableKeyLight();

	char GetASCCICharacter(uint8 iScanCode);

private:
};


#endif