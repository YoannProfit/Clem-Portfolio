//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : PIT.h
// Date de création : Octobre 2015
// Description : Une classe pour gérer les ticks
// par count (PIT system)
//----------------------------------------------------

#ifndef PROGRAMMABLE_INTERVAL_TIMER_H
#define PROGRAMMABLE_INTERVAL_TIMER_H

class PIT
{
public:
	PIT();
	virtual ~PIT();

	void Remap_IRQs();

private:
};

#endif