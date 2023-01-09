//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : Array.h
// Date de création : Octobre 2015
// Description : Une classe pour gérer une liste
// par un algorithme de liste circulaire doublement
// chaînée
//----------------------------------------------------

#ifndef LIST_H
#define LIST_H

#include "Types.h"

namespace Core
{
	template <class T>
	class List
	{
		struct Element
		{
			T value;
			Element* pPrevious;
			Element* pNext;
		};

	public:
		List();
		virtual ~List();

		void AddToHead(Element* pElem);
		void AddToTail(Element* pElem);

		Element* GetHead();
		Element* GetTail();

		void Delete(Element* pElem);

		bool IsEmpty();

		Element* Pop();

		void Clear();

	private:
	};
};

#endif