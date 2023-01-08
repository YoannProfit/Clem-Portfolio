//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : Clock.h
// Date de création : Octobre 2015
// Description : Une classe afin d'avoir l'accès à 
// l'horloge de la machine afin de récupérer la date 
// du système
//----------------------------------------------------

#ifndef CLOCK_H
#define CLOCK_H

#include "Types.h"
#include "Singleton.h"

class Clock : Singleton<Clock>
{
public:
	struct Date
	{
		uint32 iYear; 
		uint32 iMonth; 
		uint32 iDay;
	};

	struct Time
	{
		uint32 iHour; 
		uint32 iMinute; 
		uint32 iSecond;
	};

	Clock();
	virtual ~Clock();

	void InitializePort();

	Date GetDate();
	Time GetTime();

	uint32 GetYear();
	uint32 GetMonth();
	uint32 GetDay();
	uint32 GetWeekday();
	uint32 GetHour();
	uint32 GetMinute();
	uint32 GetSecond();
};

#endif