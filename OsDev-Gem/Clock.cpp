#include "Clock.h"

Clock::Clock()
{
}

Clock::~Clock()
{
}

void Clock::InitializePort()
{
	GEM_IO_PORTS->SetOutputByte(GEM_REAL_TIME_CLOCK_OUT_PORT, 0x95);
}

Date Clock::GetDate()
{
	Data date;

	date.iYear = GetYear();
	date.iMonth = GetMonth();
	date.iDay = GetDay();

	return date;
}

Time Clock::GetTime()
{	
	Time time;

	time.iHour = GetHour();
	time.iMinute = GetMinute();
	time.iSecond = GetSecond();

	return time;
}

uint32 Clock::GetYear()
{
	InitializePort();

	GEM_IO_PORTS->OutputByte(GEM_REAL_TIME_CLOCK_OUT_PORT, 0x09);

	uint32 iYear = GEM_IO_PORTS->InputByte(GEM_REAL_TIME_CLOCK_IN_PORT);

	return iYear;
}

uint32 Clock::GetMonth()
{
	InitializePort();

	GEM_IO_PORTS->OutputByte(GEM_REAL_TIME_CLOCK_OUT_PORT, 0x08);

	uint32 iMonth = GEM_IO_PORTS->InputByte(GEM_REAL_TIME_CLOCK_IN_PORT);

	return iMonth;
}

uint32 Clock::GetDay()
{
	InitializePort();

	GEM_IO_PORTS->OutputByte(GEM_REAL_TIME_CLOCK_OUT_PORT, 0x07);

	uint32 iDay = GEM_IO_PORTS->InputByte(GEM_REAL_TIME_CLOCK_IN_PORT);

	return iDay;
}

uint32 Clock::GetHour()
{
	InitializePort();

	GEM_IO_PORTS->OutputByte(GEM_REAL_TIME_CLOCK_OUT_PORT, 0x04);

	uint32 iHour = GEM_IO_PORTS->InputByte(GEM_REAL_TIME_CLOCK_IN_PORT);

	return iHour;
}

uint32 Clock::GetMinute()
{
	InitializePort();

	GEM_IO_PORTS->OutputByte(GEM_REAL_TIME_CLOCK_OUT_PORT, 0x02);

	uint32 iMinute = GEM_IO_PORTS->InputByte(GEM_REAL_TIME_CLOCK_IN_PORT);

	return iMinute;
}

uint32 Clock::GetSecond()
{
	InitializePort();

	GEM_IO_PORTS->OutputByte(GEM_REAL_TIME_CLOCK_OUT_PORT, 0x00);

	uint32 iSecond = GEM_IO_PORTS->InputByte(GEM_REAL_TIME_CLOCK_IN_PORT);

	return iSecond;
}

uint32 Clock::GetWeekday()
{
	InitializePort();

	uint32 iWeekDay = 0;
	uint32 iDataWeekday = 0;

	GEM_IO_PORTS->OutputByte(GEM_REAL_TIME_CLOCK_OUT_PORT, 0x06);

	iDataWeekday = GEM_IO_PORTS->InputByte(GEM_REAL_TIME_CLOCK_IN_PORT);

	if (iDataWeekday < 6)
	{
		iWeekDay = iDataWeekday + 2;
	}
	else 
	{
		iWeekDay = iDataWeekday - 5;
	}

	return iWeekDay;
}