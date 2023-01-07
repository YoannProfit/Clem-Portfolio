//----------------------------------------------------
// Auteur : Cl�ment Profit
// Nom du fichier : Array.h
// Date de cr�ation : Octobre 2015
// Description : Une classe pour g�rer une liste
// par un algorithme de liste circulaire doublement
// cha�n�e
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