//----------------------------------------------------
// Auteur : Cl�ment Profit
// Nom du fichier : PIT.h
// Date de cr�ation : Octobre 2015
// Description : Une classe pour g�rer les ticks
// par count (PIT system)
//----------------------------------------------------

#ifndef PROGRAMMABLE_INTERVAL_TIMER_H
#define PROGRAMMABLE_INTERVAL_TIMER_H

class PIT
{
public:
	PIT();
	virtual ~PIT();

	void RemapIRQs();

private:
};

#endif