#include "VideoMemory.h"

uint8* VideoMemory::m_aVideoRamStart = (uint8*)VIDEO_MEMORY_ADRESS;

VideoMemory::VideoMemory()
{
}

VideoMemory::~VideoMemory()
{
	Clear();
}

uint16 VideoMemory::MakeEntry(const char c, Core::Color foreground, Core::Color backgroung)
{
	uint8 iAttribute = (background << 4) | (foreground & 0x0F);

	return c | (iAttribute << 8);
}

void VideoMemory::UpdateVideoMemory()
{
	for (uint16 x = 0; x < m_aEntries.GetWidth(); x++)
	{
		for (uint16 y = 0; y < m_aEntries.GetHeight(); y++)
		{
			uint16 entry = m_aEntries[x, y];

			uint16* iWhere = m_aVideoRamStart + (y * GEM_VIDEO_MEMORY_HEIGHT + x);

			*iWhere = entry;
		}
	}
}

void VideoMemory::PutCharacter(uint32 iPosX, uint32 iPosY, const char c, 
							  Core::Color foreground, Core::Color background)
{
	AssertMsg(iPosX <= GEM_VIDEO_MEMORY_WIDTH, "Dépassement de largeur");
	AssertMsg(iPosY <= GEM_VIDEO_MEMORY_HEIGHT, "Dépassement de hauteur");

	m_aEntries[iPosX, iPosY] = MakeEntry(c, foreground, background);

	UpdateVideoMemory();
}

void PutCharacter(const TextElement& elem)
{
	PutCharacter(elem.iPosX, elem.iPosY, elem.c, elem.foreground, elem.background);
}

void VideoMemory::Clear()
{
	m_aEntries.Clear();

	uint16 blank = MakeEntry(' ', Core::Color::Black, Core::Color::Black);

	// On remplit l'écran avec que des espaces
	for (uint16 x = 0; x < m_aEntries.GetWidth(); x++)
	{
		for (uint16 y = 0; y < m_aEntries.GetHeight(); y++)
		{
			uint16* iWhere = m_aVideoRamStart + (y * GEM_VIDEO_MEMORY_HEIGHT + x);

			*iWhere = blank;
		}
	}
}

void VideoMemory::RemoveCharacter(uint32 iPosX, uint32 iPosY)
{
	AssertMsg(iPosX <= GEM_VIDEO_MEMORY_WIDTH, "Dépassement de largeur");
	AssertMsg(iPosY <= GEM_VIDEO_MEMORY_HEIGHT, "Dépassement de hauteur");

	m_aEntries.Remove(iPosX, iPosY);

	uint16 blank = MakeEntry(' ', Core::Color::Black, Core::Color::Black);

	uint16* iWhere = m_aVideoRamStart + (iPosY * GEM_VIDEO_MEMORY_HEIGHT + iPosX);

	*iWhere = blank;
}

void VideoMemory::MakeTextElement(const uint32 iPosX, const uint32 iPosY, const char c, 
			const TCore::Color foreground, const TCore::Color background)
{
	TextElement elem;

	elem.iPosX = iPosX;
	elem.iPosY = iPosX;
	elem.c = c;
	elem.foreground = foreground;
	elem.background = background;

	return elem;
}

void VideoMemory::ClearLine(uint32 iPosY)
{
	AssertMsg(iPosY <= GEM_VIDEO_MEMORY_HEIGHT, "Dépassement de hauteur");

	for (uint16 x = 0; x < m_aEntries.GetWidth(); x++)
	{
		m_aEntries.Remove(x, iPosY);

		uint16 blank = MakeEntry(' ', Core::Color::Black, Core::Color::Black);

		uint16* iWhere = m_aVideoRamStart + (iPosY * GEM_VIDEO_MEMORY_HEIGHT + x);

		*iWhere = blank;
	}
}

void VideoMemory::RefreshLine(uint32 iPosY)
{
	AssertMsg(iPosY <= GEM_VIDEO_MEMORY_HEIGHT, "Dépassement de hauteur");

	for (uint16 x = 0; x < m_aEntries.GetWidth(); x++)
	{
		uint16 entry = m_aEntries[x, iPosY];

		uint16* iWhere = m_aVideoRamStart + (iPosY * GEM_VIDEO_MEMORY_HEIGHT + x);

		*iWhere = entry;
	}
}