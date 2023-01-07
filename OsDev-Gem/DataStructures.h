#include "Types.h"
#include "Colour.h"

namespace Core
{
	struct Dimension2D
	{
		uint16 iWidth;
		uint16 iHeight;
	}

	struct Position2D
	{
		uint16 x;
		uint16 y;
	}

	struct Registers 
	{
		uint32 di; 
		uint32 si; 
		uint32 bp; 
		uint32 sp; 
		uint32 bx; 
		uint32 dx; 
		uint32 cx; 
		uint32 ax;
	};

	struct TextElement
	{
		uint32 iPosX;
		uint32 iPosY;
		char c;
		Core::Color foreground;
		Core::Color background;
	};

}