//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : VideoMemory.h
// Date de création : Octobre 2015
// Description : Classe qui représente 
// la mémoire vidéo mappée par le contrôleur vidéo
//----------------------------------------------------


#ifndef VIDEO_MEMORY_H
#define VIDEO_MEMORY_H

#include "Types.h"
#include "Defines.h"
#include "Colors.h"
#include "Singleton.h"

namespace Kernel
{
	class VideoMemory : public Singleton<VideoMemory>
	{
	public:
		VideoMemory();
		virtual ~VideoMemory();
	
		void PutCharacter(const uint32 iPosX, const uint32 iPosY, const char c, 
						  const Core::Color foreground, const Core::Color background);

		void PutCharacter(const Core::TextElement& entry);

		void Clear();

		void RemoveCharacter(uint32 iPosX, uint32 iPosY);

		static TextElement MakeTextElement();

		void ClearLine(uint32 iPosY);
		void RefreshLine(uint32 iPosY);

	private:
		uint16 MakeEntry(const char c, Core::Color foreground, Core::Color backgroung);

		void UpdateVideoMemory();

	private:
		const uint8* 				m_aVideoRamStart;

		Core::Array2D<uint16> 		m_aEntries;
	};
}

#endif
